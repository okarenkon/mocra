#ifndef TEXTDETECTOR_H
#define TEXTDETECTOR_H
#include <opencv2/opencv.hpp>

class TextDetector
{
public:
	TextDetector(
	 const cv::Mat& img,
	 const std::vector<cv::Rect>& lines,
	 const QString& credentialsFilePath,
	 QTextEdit *textlog);
	virtual ~TextDetector();

public:
	QString Text();

private:
	QTextEdit*	m_logWidget;
	QVector<QString>	m_imageFilenames;
	QString		m_resultText;
};

#endif	//TEXTDETECTOR_H
