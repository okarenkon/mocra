#include <QtWidgets>
#include <QImage>
#include "TextDetector.h"

#pragma execution_character_set("utf-8")

TextDetector::TextDetector(const cv::Mat& img, const QString& credentialsFilePath, QTextEdit *textlog)
	: m_logWidget(textlog)
	, m_imageFilename()
	, m_resultText()
{
	m_imageFilename = QStandardPaths::TempLocation;
	m_imageFilename = QDir::tempPath();
	m_imageFilename += "\\_mocr_temp.png";

	QImage qimg = QImage(img.ptr(), img.cols, img.rows, img.step, QImage::Format_RGB888).copy();
	qimg.save(m_imageFilename);

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("GOOGLE_APPLICATION_CREDENTIALS", credentialsFilePath);

	QProcess detect;
	detect.setProcessEnvironment(env);

	m_logWidget->append("Credentials file...");
	m_logWidget->append(env.value("GOOGLE_APPLICATION_CREDENTIALS"));
	m_logWidget->append("Start text detection...");
	m_logWidget->repaint();

	detect.start("textDetection");
	if (! detect.waitForStarted()) {
		m_logWidget->append("textDetection : Can not start process.");
		return;
	}

	detect.write(m_imageFilename.toLocal8Bit());
	detect.closeWriteChannel();

	if (! detect.waitForFinished()) {
		m_logWidget->append("textDetection : Process error.");
		return;
	}

	QByteArray result = detect.readAll();

	if (detect.exitCode() > 0) {
		m_logWidget->append("Completed.");

		QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
		m_resultText = codec->toUnicode(result);
	} else {
		m_logWidget->append("Error!!!");
		QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
		m_logWidget->append(codec->toUnicode(result));
	}
	m_logWidget->repaint();
}

TextDetector::~TextDetector()
{
	if (!m_imageFilename.isEmpty()) {
		QFile::remove(m_imageFilename);
	}
}

QString TextDetector::Text()
{
	return m_resultText;
}
