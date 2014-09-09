#ifndef MULTIPAGEIMAGEREADER_H
#define MULTIPAGEIMAGEREADER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <tiffio.h>

//! �}���`�y�[�W�摜�ǂݍ��݃N���X
class MultiPageImageReader
{
public:
	//! �R���X�g���N�^
	MultiPageImageReader(
	 const std::string& fileName);	//!> �摜�t�@�C����
	//! �f�X�g���N�^
	virtual ~MultiPageImageReader();

	//! �摜�ǂݍ���
	cv::Mat read(
	 bool* eof = NULL);				//!> EOF�t���O

private:
	std::string	m_fileName;		//!> �t�@�C����
	TIFF*		m_pTiff;		//!> TIFF�C���[�W(TIFF�t�@�C���ȊO��NULL)
	int			m_pageCount;	//!> �y�[�W�J�E���^
};

#endif	//MULTIPAGEIMAGEREADER_H

