#ifndef TEXTDETECTOR_H
#define TEXTDETECTOR_H
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QString>
#include <QVector>

class TextDetector : public QObject
{
	Q_OBJECT

public:
	TextDetector(
	 const cv::Mat& img,
	 const std::vector<cv::Rect>& lines,
	 const QString& credentialsFilePath,
	 const QObject* parent);
	virtual ~TextDetector();

public:
	QString Text();

signals:
	void log(const QString&);

private:
	QVector<QString>	m_imageFilenames;
	QString		m_resultText;
};

#endif	//TEXTDETECTOR_H
