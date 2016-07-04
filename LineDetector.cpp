#include "Labeling.h"
#include "TRange.h"
#include "LineDetector.h"

//! �R���X�g���N�^
LineDetector::LineDetector(
 const cv::Mat& imgGray)				//!> ���͉摜
	:	m_lines()
{
	//�s�F��
	m_lines = getLineRects(imgGray);
}

//! �f�X�g���N�^
LineDetector::~LineDetector()
{
}

//! �s�擾
std::vector<cv::Rect> LineDetector::lines() const
{
	return m_lines;
}

//! �s�F��
std::vector<cv::Rect> LineDetector::getLineRects(
 const cv::Mat& imgGray) const			//!> ���͉摜
{
	std::vector<cv::Rect> rects;

	//��l��
	cv::Mat imgBin;
	cv::threshold(imgGray, imgBin, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

#if 0
	cv::namedWindow("��l��", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("��l��", imgBin);
	cv::waitKey(0);
	cv::destroyWindow("��l��");
#endif

	//��l�������摜�ɑ΂��ă��x�����O�����s����
	cv::Mat label(imgBin.size(), CV_16SC1);	//���x�����O����
	LabelingBS labeling;
	labeling.Exec(imgBin.data, (short *)label.data, imgBin.cols, imgBin.rows, false, 0);
	
	//���x�����O���ʂ�cv::Rect��vector�ɃR�s�[
	for (int i = 0; i < labeling.GetNumOfRegions(); i++) {
		RegionInfoBS* info1 = labeling.GetResultRegionInfo(i);
		
		cv::Point p1;		//��`�̍�����W
		info1->GetMin(p1.x, p1.y);
		cv::Point p2;		//��`�̉E�����W
		info1->GetMax(p2.x, p2.y);
		
		rects.push_back(cv::Rect(p1, p2));
	}

	//�����̍~���Ƀ\�[�g
	std::sort(rects.begin(), rects.end(), GreaterRectHeight());

	//�c�͈̔͂��d�Ȃ��Ă����`���������ɓ������Ă���
	while (! rects.empty()) {
		size_t count = 0;	//��`�𓝍�������

		for (size_t i = 0; i < rects.size() - 1; i++) {
			for (size_t j = i + 1; j < rects.size(); j++) {
				TRange<double> r1(rects[i].y, rects[i].y + rects[i].height);	//�傫����`�̏c�͈̔�
				TRange<double> r2(rects[j].y, rects[j].y + rects[j].height);	//��������`�̏c�͈̔�
				TRange<double> rc = r1.intersected(r2);							//�c�͈̔͂��d�Ȃ��Ă���͈�
		
				if (! rc.isNull()) {
					if (rc.size() >= r2.size() * 0.5) {
						//�c�͈̔͂��d�Ȃ��Ă���͈͂ɁA��������`�̏c�͈̔͂�50%�ȏオ�d�Ȃ��Ă����瓝��
						rects[i] |= rects[j];
						count++;

						//����������`���폜
						rects.erase(rects.begin() + j);
					}
				}
			}
		}

		//�d�Ȃ荇����`���Ȃ��Ȃ�����I���
		if (count == 0) {
			break;
		}
	}

	//Y���W�̏����Ƀ\�[�g
	std::sort(rects.begin(), rects.end(), LessRectY());

#if 1	//�s���o���ʂ�\��
	cv::Mat imgDebug;
	cv::cvtColor(imgGray, imgDebug, CV_GRAY2RGB);

	//�̈��`��
	for (size_t i = 0; i < rects.size(); i++) {
		cv::rectangle(imgDebug, rects[i], CV_RGB(255, 0, 0));
	}

	cv::namedWindow("�s���o����", CV_WINDOW_AUTOSIZE);
	cv::imshow("�s���o����", imgDebug);
	cv::waitKey(0);
	cv::destroyWindow("�s���o����");
#endif

	//�s�̋�`�̔z���Ԃ�
	return rects;
}
