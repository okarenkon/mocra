#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <opencv2/opencv.hpp>

//! 行認識クラス
class LineDetector
{
public:
	//! コンストラクタ
	LineDetector(
	 const cv::Mat& imgGray);				//!> 入力画像

	//! デストラクタ
	~LineDetector();

	//! 行取得
	std::vector<cv::Rect> lines() const;

private:
	//! 行認識
	std::vector<cv::Rect> getLineRects(
	 const cv::Mat& imgGray) const;			//!> 入力画像

private:
	std::vector<cv::Rect> m_lines;			//!> 認識行の矩形領域
};

class GreaterRectHeight
{
public:
    bool operator() (const cv::Rect& lhs, const cv::Rect& rhs) const
	{
        return lhs.height > rhs.height;
    }
};

class LessRectY
{
public:
    bool operator() (const cv::Rect& lhs, const cv::Rect& rhs) const
	{
        return lhs.y < rhs.y;
    }
};

#endif	//LINEDETECTOR_H
