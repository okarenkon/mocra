#ifndef SLANTCORRECTOR_H
#define SLANTCORRECTOR_H

#include <opencv2/opencv.hpp>

//! 傾き補正クラス
class SlantCorrector
{
public:
	//! コンストラクタ
	SlantCorrector(
	 const cv::Mat& imgGray);			//! >入力画像

	//! デストラクタ
	~SlantCorrector();

	//! 傾き角度取得
	double slant() const;

	//! 傾き補正画像取得
	cv::Mat correctedImage() const;

private:
	//! 傾き検出
	double detectSlant(
	 const cv::Mat& imgGray) const;		//!> 入力画像

	//! 傾き補正
	cv::Mat correctsSlant(
	 const cv::Mat& imgGray,			//!> 入力画像
	 double slant) const;				//!> 傾き角度

private:
	double	m_slant;					//!> 傾き角度
	cv::Mat	m_imgCorrected;				//!> 傾き補正後画像
};

#endif	//SLANTCORRECTOR_H
