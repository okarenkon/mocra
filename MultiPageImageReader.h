#ifndef MULTIPAGEIMAGEREADER_H
#define MULTIPAGEIMAGEREADER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <tiffio.h>

class MultiPageImageReader
{
public:
	MultiPageImageReader(const std::string& fileName);
	virtual ~MultiPageImageReader();

	cv::Mat read(bool* eof = NULL);

private:
	std::string	m_fileName;		//�t�@�C����
	TIFF*		m_pTiff;		//TIFF�t�@�C���ȊO��NULL
	int			m_pageCount;	//�y�[�W�J�E���^
};

#endif	//MULTIPAGEIMAGEREADER_H

