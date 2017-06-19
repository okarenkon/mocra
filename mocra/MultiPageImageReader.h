#ifndef MULTIPAGEIMAGEREADER_H
#define MULTIPAGEIMAGEREADER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <tiffio.h>

//! マルチページ画像読み込みクラス
class MultiPageImageReader
{
public:
	//! コンストラクタ
	MultiPageImageReader(
	 const std::string& fileName);	//!> 画像ファイル名
	//! デストラクタ
	virtual ~MultiPageImageReader();

	//! 画像読み込み
	cv::Mat read(
	 bool* eof = NULL);				//!> EOFフラグ

private:
	std::string	m_fileName;		//!> ファイル名
	TIFF*		m_pTiff;		//!> TIFFイメージ(TIFFファイル以外はNULL)
	int			m_pageCount;	//!> ページカウンタ
};

#endif	//MULTIPAGEIMAGEREADER_H

