#include "Labeling.h"
#include "TRange.h"
#include "LineDetector.h"

//! コンストラクタ
LineDetector::LineDetector(
 const cv::Mat& imgGray)				//!> 入力画像
	:	m_lines()
{
	//行認識
	m_lines = getLineRects(imgGray);
}

//! デストラクタ
LineDetector::~LineDetector()
{
}

//! 行取得
std::vector<cv::Rect> LineDetector::lines() const
{
	return m_lines;
}

//! 行認識
std::vector<cv::Rect> LineDetector::getLineRects(
 const cv::Mat& imgGray) const			//!> 入力画像
{
	std::vector<cv::Rect> rects;

	//二値化
	cv::Mat imgBin;
	cv::threshold(imgGray, imgBin, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

#if 0
	cv::namedWindow("二値化", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("二値化", imgBin);
	cv::waitKey(0);
	cv::destroyWindow("二値化");
#endif

	//二値化した画像に対してラベリングを実行する
	cv::Mat label(imgBin.size(), CV_16SC1);	//ラベリング結果
	LabelingBS labeling;
	labeling.Exec(imgBin.data, (short *)label.data, imgBin.cols, imgBin.rows, false, 0);
	
	//ラベリング結果をcv::Rectのvectorにコピー
	for (int i = 0; i < labeling.GetNumOfRegions(); i++) {
		RegionInfoBS* info1 = labeling.GetResultRegionInfo(i);
		
		cv::Point p1;		//矩形の左上座標
		info1->GetMin(p1.x, p1.y);
		cv::Point p2;		//矩形の右下座標
		info1->GetMax(p2.x, p2.y);
		
		rects.push_back(cv::Rect(p1, p2));
	}

	//高さの降順にソート
	std::sort(rects.begin(), rects.end(), GreaterRectHeight());

	//縦の範囲が重なっている矩形を横方向に統合していく
	while (! rects.empty()) {
		size_t count = 0;	//矩形を統合した回数

		for (size_t i = 0; i < rects.size() - 1; i++) {
			for (size_t j = i + 1; j < rects.size(); j++) {
				TRange<double> r1(rects[i].y, rects[i].y + rects[i].height);	//大きい矩形の縦の範囲
				TRange<double> r2(rects[j].y, rects[j].y + rects[j].height);	//小さい矩形の縦の範囲
				TRange<double> rc = r1.intersected(r2);							//縦の範囲が重なっている範囲
		
				if (! rc.isNull()) {
					if (rc.size() >= r2.size() * 0.5) {
						//縦の範囲が重なっている範囲に、小さい矩形の縦の範囲の50%以上が重なっていたら統合
						rects[i] |= rects[j];
						count++;

						//統合した矩形を削除
						rects.erase(rects.begin() + j);
					}
				}
			}
		}

		//重なり合う矩形がなくなったら終わる
		if (count == 0) {
			break;
		}
	}

	//Y座標の昇順にソート
	std::sort(rects.begin(), rects.end(), LessRectY());

#if 1	//行検出結果を表示
	cv::Mat imgDebug;
	cv::cvtColor(imgGray, imgDebug, CV_GRAY2RGB);

	//領域を描画
	for (size_t i = 0; i < rects.size(); i++) {
		cv::rectangle(imgDebug, rects[i], CV_RGB(255, 0, 0));
	}

	cv::namedWindow("行検出結果", CV_WINDOW_AUTOSIZE);
	cv::imshow("行検出結果", imgDebug);
	cv::waitKey(0);
	cv::destroyWindow("行検出結果");
#endif

	//行の矩形の配列を返す
	return rects;
}
