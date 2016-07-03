#include "Labeling.h"
#include "SlantCorrector.h"

//! �R���X�g���N�^
SlantCorrector::SlantCorrector(
 const cv::Mat& imgGray)			//! >���͉摜
	:	m_slant(DBL_MAX),
		m_imgCorrected()
{
	//�X�����o
	m_slant = detectSlant(imgGray);

	//�X���␳
	m_imgCorrected = correctsSlant(imgGray, m_slant);
}

//! �f�X�g���N�^
SlantCorrector::~SlantCorrector()
{
}

//! �X���p�x�擾
double SlantCorrector::slant() const
{
	return m_slant;
}

//! �X���␳�摜�擾
cv::Mat SlantCorrector::correctedImage() const
{
	return m_imgCorrected;
}

//! �X�����o
double SlantCorrector::detectSlant(
 const cv::Mat& imgGray) const		//!> ���͉摜
{
	//��l��
	cv::Mat imgBin;
	cv::threshold(imgGray, imgBin, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

#if 1
	cv::namedWindow("��l��", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("��l��", imgBin);
	cv::waitKey(0);
	cv::destroyWindow("��l��");
#endif

	//��l�������摜�ɑ΂��ă��x�����O�����s
	cv::Mat label(imgBin.size(), CV_16SC1);	//���x�����O����
	LabelingBS	labeling;
	labeling.Exec(imgBin.data, (short *)label.data, imgBin.cols, imgBin. rows, false, 0);

#if 1	//���x�����O���ʂ�\��
{
	cv::Mat imgDebug;
	cv::cvtColor(imgGray, imgDebug, CV_GRAY2RGB);

	//���x�����O���ʂ�`��
	for (int i = 0; i < labeling.GetNumOfRegions(); i++) {
		RegionInfoBS* info = labeling.GetResultRegionInfo(i);
		cv::Point p1;		//��`�̍�����W
		info->GetMin(p1.x, p1.y);
		cv::Point p2;		//��`�̉E�����W
		info->GetMax(p2.x, p2.y);

		cv::rectangle(imgDebug, p1, p2, CV_RGB(255, 0, 0));
	}

	cv::namedWindow("���x�����O����", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("���x�����O����", imgDebug);
	cv::waitKey(0);
	cv::destroyWindow("���x�����O����");
}
#endif

	//�e�̈�̉��Ӓ����̓_���l�摜�ɕ`��
	cv::Mat tmp = cv::Mat::zeros(imgBin.size(), CV_8UC1);
	for (int i = 0; i < labeling.GetNumOfRegions(); i++) {
		RegionInfoBS* info = labeling.GetResultRegionInfo(i);

		cv::Point p1;		//��`�̍�����W
		info->GetMin(p1.x, p1.y);
		cv::Point p2;		//��`�̉E�����W
		info->GetMax(p2.x, p2.y);

		//���Ӓ����̓_��`��
		cv::line(tmp, cv::Point((p1.x + p2.x) / 2, p2.y), cv::Point((p1.x + p2.x) / 2, p2.y), 255);
	}

#if 1	//�e�̈�̉��Ӓ����̓_��\��
	cv::namedWindow("���Ӓ����̓_", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("���Ӓ����̓_", tmp);
	cv::waitKey(0);
	cv::destroyWindow("���Ӓ����̓_");
#endif

	//���l�����X�Ɍ��������Ȃ��璼�����o
	//�ϓ��W�������ȉ��ɂȂ�����ł��؂�
	double slant = DBL_MAX;
	std::vector<cv::Vec2f>	lines;

	for (int thre = 2; thre < labeling.GetNumOfRegions(); thre++) {
		//�������o
		cv::HoughLines(tmp, lines, 1, CV_PI/180/10, thre);
		if (lines.empty()) {
			return DBL_MAX;		//�X�����o�s��
		}

#if 1	//�������o���ʂ�\��
{
		cv::Mat imgDebug;
		cv::cvtColor(imgGray, imgDebug, CV_GRAY2RGB);

		//�������o���ʂ�`��
		for (size_t i = 0; i < lines.size(); i++) {
			float rho = lines[i][0];
			float theta = lines[i][1];
			double a = cos(theta);
			double b = sin(theta);
			double x0 = a*rho;
			double y0 = b*rho;
			cv::Point pt1(	cvRound(x0 + imgDebug.cols*(-b)),
							cvRound(y0 + imgDebug.cols*(a)));
			cv::Point pt2(	cvRound(x0 - imgDebug.cols*(-b)),
							cvRound(y0 - imgDebug.cols*(a)));
			cv::line(imgDebug, pt1, pt2, 255, 1, 8);
		}

		cv::namedWindow("�������o", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
		cv::imshow("�������o", imgDebug);
		cv::waitKey(0);
		cv::destroyWindow("�������o");
}
#endif

		//theta�̕��ς����߂�
		float ave = 0.0;
		for (size_t i = 0; i < lines.size(); i++) {
			ave += lines[i][1];
		}
		ave = ave / lines.size();

		//�ϓ��W�������߂�
		float sigma = 0.0;
		for (size_t i = 0; i < lines.size(); i++) {
			sigma += pow(lines[i][1] - ave, (float)2.0);
		}
		sigma = sqrt(sigma / lines.size());

		//�ϓ��W�������߂�
		float cv = sigma / ave;
		printf("thre:%d, �ϓ��W��:%f, ���ϊp�x:%f�x, ���̐�:%d, �̈搔:%d\n", thre, cv, ave * 180.0 / CV_PI, lines.size(), labeling.GetNumOfRegions());

		//�ϓ��W����臒l�����ɂȂ�����theta�̕��ς��X���Ƃ���
		if (cv < 0.002) {
			slant = ave;
			break;
		}
	}

	return slant;
}

//! �X���␳
cv::Mat SlantCorrector::correctsSlant(
 const cv::Mat& imgGray,			//!> ���͉摜
 double slant) const				//!> �X���p�x
{
	//���摜���R�s�[
	cv::Mat imgCorrectedSlant = imgGray.clone();

	//�X���p�x������Ή�]���ĕ␳
	if ((slant != 0.0)
	 && (slant != DBL_MAX)) {
		//��]�p[deg]
		double angle = (slant * 180 / CV_PI) - 90.0;
		//��]���S
		cv::Point2f center((float)(imgGray.cols * 0.5), (float)(imgGray.rows * 0.5));

		//�ȏ�̏�������2�����̉�]�s����v�Z
		cv::Mat affine_matrix = cv::getRotationMatrix2D(center, angle, 1.0);

		//��]
		cv::warpAffine(imgGray, imgCorrectedSlant, affine_matrix, imgGray.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
	}

#if 1	//�X���␳���ʂ�\��
	cv::namedWindow("�X���␳����", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("�X���␳����", imgCorrectedSlant);
	cv::waitKey(0);
	cv::destroyWindow("�X���␳����");
#endif
	
	return imgCorrectedSlant;
}
