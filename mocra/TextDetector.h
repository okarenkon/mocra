#ifndef TEXTDETECTOR_H
#define TEXTDETECTOR_H
#include <opencv2/opencv.hpp>

class TextDetector
{
public:
	TextDetector(const cv::Mat& img, const QString& credentialsFilePath, QTextEdit *textlog);
	virtual ~TextDetector();

public:
	QString Text();

private:
	QTextEdit*	m_logWidget;
	QString		m_imageFilename;
	QString		m_resultText;
};

#endif	//TEXTDETECTOR_H
