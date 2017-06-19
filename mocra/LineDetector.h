#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <vector>
#include <opencv2/opencv.hpp>

//! �s�F���N���X
class LineDetector
{
public:
	//! �R���X�g���N�^
	LineDetector(
	 const cv::Mat& imgGray);				//!> ���͉摜

	//! �f�X�g���N�^
	~LineDetector();

	//! �s�擾
	std::vector<cv::Rect> lines() const;

private:
	//! �s�F��
	std::vector<cv::Rect> getLineRects(
	 const cv::Mat& imgGray) const;			//!> ���͉摜

private:
	std::vector<cv::Rect> m_lines;			//!> �F���s�̋�`�̈�
};

//! ��`�����~���\�[�g�p��r�N���X
class GreaterRectHeight
{
public:
    bool operator() (const cv::Rect& lhs, const cv::Rect& rhs) const
	{
        return lhs.height > rhs.height;
    }
};

//! ��`Y���W�����\�[�g�p��r�N���X
class LessRectY
{
public:
    bool operator() (const cv::Rect& lhs, const cv::Rect& rhs) const
	{
        return lhs.y < rhs.y;
    }
};

#endif	//LINEDETECTOR_H
