#include <opencv2/opencv.hpp>
#include "MultiPageImageReader.h"

bool doOCR(const std::string& fileName);

int main(int argc, char* argv[])
{
	char* files[] = {
		"../../../jpeg-6b/testimg.bmp",
		"../../../jpeg-6b/testimg.jpg",
		"../../../jpeg-6b/testimg.ppm",
		"../../../jpeg-6b/testimgp.jpg",
		"../../../jpeg-6b/testorig.jpg",
		"../../../jpeg-6b/testprog.jpg",
		"../../../tiff-4.0.3/test/images/minisblack-1c-16b.tiff",
		"../../../tiff-4.0.3/test/images/minisblack-1c-8b.tiff",
		"../../../tiff-4.0.3/test/images/miniswhite-1c-1b.tiff",
		"../../../tiff-4.0.3/test/images/palette-1c-1b.tiff",
		"../../../tiff-4.0.3/test/images/palette-1c-4b.tiff",
		"../../../tiff-4.0.3/test/images/palette-1c-8b.tiff",
		"../../../tiff-4.0.3/test/images/rgb-3c-16b.tiff",
		"../../../tiff-4.0.3/test/images/rgb-3c-8b.tiff",
		"../../../tiff-4.0.3/test/images/logluv-3c-16b.tiff",
		"../../../tiff-4.0.3/test/images/minisblack-2c-8b-alpha.tiff",
		"../../../tiff-4.0.3/test/images/palette-1c-8b.bmp",
		"../../../tiff-4.0.3/test/images/rgb-3c-8b.bmp",
		"../../../tiff-4.0.3/test/images/palette-1c-8b.gif",
		"../../../tiff-4.0.3/test/images/minisblack-1c-8b.pgm",
		"../../../tiff-4.0.3/test/images/miniswhite-1c-1b.pbm",
		"../../../tiff-4.0.3/test/images/rgb-3c-8b.ppm",
		"../../SIDBA/color/LENNA.bmp",
		"../../SIDBA/mono/LENNA.bmp",
		"../../pics-3.6.1/caspian.tif",
		"../../pics-3.6.1/cramps.tif",
		"../../pics-3.6.1/cramps-tile.tif",
		"../../pics-3.6.1/fax2d.tif",
		"../../pics-3.6.1/g3test.g3",
		"../../pics-3.6.1/g3test.tif",
		"../../pics-3.6.1/jello.tif",
		"../../pics-3.6.1/off_l16.tif",
		"../../pics-3.6.1/off_luv24.tif",
		"../../pics-3.6.1/off_luv32.tif",
		"../../pics-3.6.1/quad-jpeg.tif",
		"../../pics-3.6.1/quad-lzw.tif",
		"../../pics-3.6.1/quad-tile.tif",
		"../../pics-3.6.1/strike.tif",
		"../../pics-3.6.1/text.tif",		//multi page
		"../../pics-3.6.1/ycbcr-cat.tif",
		"../../pics-3.6.1/smallliz.tif",
		"../../pics-3.6.1/zackthecat.tif",
		"../../pics-3.6.1/oxford.tif",
		"../../pics-3.6.1/jim___cg.tif",
		"../../pics-3.6.1/jim___dg.tif",
		"../../pics-3.6.1/jim___gg.tif",
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

		//OpenCVのウィンドウに画像を表示
		cv::namedWindow(fileName, CV_WINDOW_AUTOSIZE);
		cv::imshow(fileName, img);
		cv::waitKey(0);
		cv::destroyWindow(fileName);
	}

	return true;	//正常
}
