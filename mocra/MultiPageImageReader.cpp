#include <cstdio>
#include "MultiPageImageReader.h"

//! �R���X�g���N�^
MultiPageImageReader::MultiPageImageReader(
 const std::string& fileName)		//!> �摜�t�@�C����
	: m_fileName(fileName),
	  m_pTiff(NULL),
	  m_pageCount(0)
{
	//�Ƃ肠����TIFF�t�@�C���O��ŊJ���Ă���
	//(TIFF�t�@�C�����ǂ����̔��f��libtiff�ɔC����������)
	m_pTiff = ::TIFFOpen(fileName.c_str(), "r");
}

//! �f�X�g���N�^
MultiPageImageReader::~MultiPageImageReader()
{
	if (m_pTiff != NULL) {
		::TIFFClose(m_pTiff);
	}
}

//! �摜�ǂݍ���
cv::Mat MultiPageImageReader::read(
 bool* eof/* = NULL*/)				//!> EOF�t���O
{
	if (eof != NULL) {
		*eof = false;
	}

	if (m_pTiff != NULL) {
		//libtiff�ŊJ���Ă�����libtiff�œǂݍ���

		if (m_pageCount > 0) {
			//���̃y�[�W��
			if (! ::TIFFReadDirectory(m_pTiff)) {
				if (eof != NULL) {
					*eof = true;
				}
				::TIFFClose(m_pTiff);
				m_pTiff = NULL;
				return cv::Mat();
			}
		}

		uint32 w = 0;	//�摜�̕�[pix]
		uint32 h = 0;	//�摜�̍���[pix]

		//�摜�̕��A�����擾
		::TIFFGetField(m_pTiff, TIFFTAG_IMAGEWIDTH, &w);
		::TIFFGetField(m_pTiff, TIFFTAG_IMAGELENGTH, &h);

		//�摜�S�̂̃o�b�t�@���m��(BGRA)
		cv::Mat image(h, w, CV_8UC4);

		//�ǂ�ȃJ���[�t�H�[�}�b�g�ł����ׂ�RGBA�`���ɕϊ����ēǂݍ���
		::TIFFReadRGBAImage(m_pTiff, w, h, (uint32*)image.data);

		//OpenCV�̉摜�`���ɍ��킹��
		cv::cvtColor(image, image, CV_RGBA2BGR);	//RGBA��BGR�ϊ�
		cv::flip(image, image, 0);	//�㉺���]

		m_pageCount++;
		return image;
	} else {
		//libtiff�ŊJ���Ă��Ȃ����OpenCV�œǂݍ���

		if (m_pageCount > 0) {
			if (eof != NULL) {
				*eof = true;
			}
			return cv::Mat();
		}

		m_pageCount++;
		return cv::imread(m_fileName, CV_LOAD_IMAGE_COLOR);
	}
}
