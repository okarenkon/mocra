#include <opencv2/opencv.hpp>
#include "MultiPageImageReader.h"
#include "SlantCorrector.h"
#include "LineDetector.h"

bool doOCR(const std::string& fileName);

int main(int argc, char* argv[])
{
	char* files[] = {
		//"sample/test_MSゴシック12_gray_200dpi.tif",
		"../sample/test_MSゴシック12_gray_200dpi_5deg.png",
		//"../../../jpeg-6b/testimg.bmp",
		//"../../../jpeg-6b/testimg.jpg",
		//"../../../jpeg-6b/testimg.ppm",
		//"../../../jpeg-6b/testimgp.jpg",
		//"../../../jpeg-6b/testorig.jpg",
		//"../../../jpeg-6b/testprog.jpg",
		//"../../../tiff-4.0.3/test/images/minisblack-1c-16b.tiff",
		//"../../../tiff-4.0.3/test/images/minisblack-1c-8b.tiff",
		//"../../../tiff-4.0.3/test/images/miniswhite-1c-1b.tiff",
		//"../../../tiff-4.0.3/test/images/palette-1c-1b.tiff",
		//"../../../tiff-4.0.3/test/images/palette-1c-4b.tiff",
		//"../../../tiff-4.0.3/test/images/palette-1c-8b.tiff",
		//"../../../tiff-4.0.3/test/images/rgb-3c-16b.tiff",
		//"../../../tiff-4.0.3/test/images/rgb-3c-8b.tiff",
		//"../../../tiff-4.0.3/test/images/logluv-3c-16b.tiff",
		//"../../../tiff-4.0.3/test/images/minisblack-2c-8b-alpha.tiff",
		//"../../../tiff-4.0.3/test/images/palette-1c-8b.bmp",
		//"../../../tiff-4.0.3/test/images/rgb-3c-8b.bmp",
		//"../../../tiff-4.0.3/test/images/palette-1c-8b.gif",
		//"../../../tiff-4.0.3/test/images/minisblack-1c-8b.pgm",
		//"../../../tiff-4.0.3/test/images/miniswhite-1c-1b.pbm",
		//"../../../tiff-4.0.3/test/images/rgb-3c-8b.ppm",
		//"../../SIDBA/color/LENNA.bmp",
		//"../../SIDBA/mono/LENNA.bmp",
		//"../../pics-3.6.1/caspian.tif",
		//"../../pics-3.6.1/cramps.tif",
		//"../../pics-3.6.1/cramps-tile.tif",
		//"../../pics-3.6.1/fax2d.tif",
		//"../../pics-3.6.1/g3test.g3",
		//"../../pics-3.6.1/g3test.tif",
		//"../../pics-3.6.1/jello.tif",
		//"../../pics-3.6.1/off_l16.tif",
		//"../../pics-3.6.1/off_luv24.tif",
		//"../../pics-3.6.1/off_luv32.tif",
		//"../../pics-3.6.1/quad-jpeg.tif",
		//"../../pics-3.6.1/quad-lzw.tif",
		//"../../pics-3.6.1/quad-tile.tif",
		//"../../pics-3.6.1/strike.tif",
		//"../../pics-3.6.1/text.tif",		//multi page
		//"../../pics-3.6.1/ycbcr-cat.tif",
		//"../../pics-3.6.1/smallliz.tif",
		//"../../pics-3.6.1/zackthecat.tif",
		//"../../pics-3.6.1/oxford.tif",
		//"../../pics-3.6.1/jim___cg.tif",
		//"../../pics-3.6.1/jim___dg.tif",
		//"../../pics-3.6.1/jim___gg.tif",
	};
	
	for (int i = 0; i < _countof(files); i++) {
		doOCR(files[i]);
	}

	return 0;
}

bool doOCR(const std::string& fileName)
{
	MultiPageImageReader mpi(fileName);

	for (;;) {
		bool eof = false;

		//画像を1ページ読み込む
		cv::Mat img = mpi.read(&eof);
		if (img.empty()) {
			if (! eof) {
				return false;	//エラー
			}
			//最後まで読んでいたらループ抜ける
			break;
		}

		//256グレースケールに変換
		cv::Mat gray;
		cv::cvtColor(img, gray, CV_BGR2GRAY);
//cv::namedWindow(fileName, CV_WINDOW_AUTOSIZE);
//cv::imshow(fileName, img);
//cv::waitKey(0);
//cv::destroyWindow(fileName);

		//ノイズ除去
		cv::Mat denoize;
		cv::adaptiveBilateralFilter(gray, denoize, cv::Size(7, 7), 7.0, 35.0);

//		//ゴマ塩ノイズ除去
//		cv::Mat element = (cv::Mat_<uchar>(3,3)<< 0,1,0,1,1,1,0,1,0); 
//		cv::erode(denoize, denoize, element);	//収縮
//		cv::dilate(denoize, denoize, element);	//膨張
//		cv::morphologyEx(denoize, denoize, cv::MORPH_CLOSE, element);

		//OpenCVのウィンドウに画像を表示
		//cv::namedWindow(fileName, CV_WINDOW_AUTOSIZE);
		//cv::imshow(fileName, denoize);
		//cv::waitKey(0);
		//cv::destroyWindow(fileName);

		//傾き補正
		SlantCorrector sc(denoize);
		cv::Mat imgCorrected = sc.correctedImage();

		//行認識
		LineDetector ld(imgCorrected);
		std::vector<cv::Rect> lines = ld.lines();
	}

	return true;	//正常
}
