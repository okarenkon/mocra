#ifndef SLANTCORRECTOR_H
#define SLANTCORRECTOR_H

#include <opencv2/opencv.hpp>

//! �X���␳�N���X
class SlantCorrector
{
public:
	//! �R���X�g���N�^
	SlantCorrector(
	 const cv::Mat& imgGray);			//! >���͉摜

	//! �f�X�g���N�^
	~SlantCorrector();

	//! �X���p�x�擾
	double slant() const;

	//! �X���␳�摜�擾
	cv::Mat correctedImage() const;

private:
	//! �X�����o
	double detectSlant(
	 const cv::Mat& imgGray) const;		//!> ���͉摜

	//! �X���␳
	cv::Mat correctsSlant(
	 const cv::Mat& imgGray,			//!> ���͉摜
	 double slant) const;				//!> �X���p�x

private:
	double	m_slant;					//!> �X���p�x
	cv::Mat	m_imgCorrected;				//!> �X���␳��摜
};

#endif	//SLANTCORRECTOR_H
