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
	std::string	m_fileName;		//ファイル名
	TIFF*		m_pTiff;		//TIFFファイル以外はNULL
	int			m_pageCount;	//ページカウンタ
};

#endif	//MULTIPAGEIMAGEREADER_H

