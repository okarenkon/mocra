#include <cstdio>
#include "MultiPageImageReader.h"

//! コンストラクタ
MultiPageImageReader::MultiPageImageReader(
 const std::string& fileName)		//!> 画像ファイル名
	: m_fileName(fileName),
	  m_pTiff(NULL),
	  m_pageCount(0)
{
	//とりあえずTIFFファイル前提で開いておく
	//(TIFFファイルかどうかの判断をlibtiffに任せたいため)
	m_pTiff = ::TIFFOpen(fileName.c_str(), "r");
}

//! デストラクタ
MultiPageImageReader::~MultiPageImageReader()
{
	if (m_pTiff != NULL) {
		::TIFFClose(m_pTiff);
	}
}

//! 画像読み込み
cv::Mat MultiPageImageReader::read(
 bool* eof/* = NULL*/)				//!> EOFフラグ
{
	if (eof != NULL) {
		*eof = false;
	}

	if (m_pTiff != NULL) {
		//libtiffで開けていたらlibtiffで読み込む

		if (m_pageCount > 0) {
			//次のページへ
			if (! ::TIFFReadDirectory(m_pTiff)) {
				if (eof != NULL) {
					*eof = true;
				}
				::TIFFClose(m_pTiff);
				m_pTiff = NULL;
				return cv::Mat();
			}
		}

		uint32 w = 0;	//画像の幅[pix]
		uint32 h = 0;	//画像の高さ[pix]

		//画像の幅、高さ取得
		::TIFFGetField(m_pTiff, TIFFTAG_IMAGEWIDTH, &w);
		::TIFFGetField(m_pTiff, TIFFTAG_IMAGELENGTH, &h);

		//画像全体のバッファを確保(BGRA)
		cv::Mat image(h, w, CV_8UC4);

		//どんなカラーフォーマットでもすべてRGBA形式に変換して読み込む
		::TIFFReadRGBAImage(m_pTiff, w, h, (uint32*)image.data);

		//OpenCVの画像形式に合わせる
		cv::cvtColor(image, image, CV_RGBA2BGR);	//RGBA→BGR変換
		cv::flip(image, image, 0);	//上下反転

		m_pageCount++;
		return image;
	} else {
		//libtiffで開けていなければOpenCVで読み込む

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
