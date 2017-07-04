#include <QtWidgets>
#include <QImage>
#include "TextDetector.h"

#pragma execution_character_set("utf-8")

TextDetector::TextDetector(
 const cv::Mat& img,
 const std::vector<cv::Rect>& lines,
 const QString& credentialsFilePath,
 const QObject* parent)
	: m_imageFilenames()
	, m_resultText()
{
	QObject::connect(this, SIGNAL(log(const QString&)), parent, SLOT(log(const QString&)));

	if (lines.size() == 0) {
		QString imageFilename = QString("%1/~$mocra_temp.png").arg(QDir::tempPath());
		cv::imwrite(QDir::toNativeSeparators(imageFilename).toStdString(), img);

		m_imageFilenames.append(imageFilename);
	}
	else {
		for (size_t i = 0; i < lines.size(); i++) {
			QString imageFilename = QString("%1/~$mocra_temp_%2.png").arg(QDir::tempPath()).arg(i, 5, 10, QChar('0'));
			cv::Mat imgLine(img, lines[i]);
			cv::imwrite(QDir::toNativeSeparators(imageFilename).toStdString(), imgLine);

			m_imageFilenames.append(imageFilename);
		}
	}

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("GOOGLE_APPLICATION_CREDENTIALS", credentialsFilePath);

	QProcess detect;
	detect.setProcessEnvironment(env);

	detect.start("textDetection");
	if (! detect.waitForStarted()) {
		emit log(tr("テキスト認識プロセスを開始できませんでした。"));
		return;
	}

	for (int i = 0; i < m_imageFilenames.count(); i++) {
		QString lineText;
		lineText = m_imageFilenames[i] + "\n";
		detect.write(lineText.toLocal8Bit());
	}
	detect.closeWriteChannel();

	if (! detect.waitForFinished()) {
		emit log(tr("テキスト認識プロセスでエラーが発生しました。"));
		return;
	}

	QByteArray result = detect.readAll();

	if (detect.exitCode() == 0) {
		emit log(tr("正常終了"));

		QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
		m_resultText = codec->toUnicode(result);
	} else {
		emit log(tr("エラー発生!!!"));

		QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
		emit log(codec->toUnicode(result));
	}
}

TextDetector::~TextDetector()
{
	for (int i = 0; i < m_imageFilenames.count(); i++) {
		if (!m_imageFilenames[i].isEmpty()) {
			if (QFile::exists(m_imageFilenames[i])) {
				QFile::remove(m_imageFilenames[i]);
			}
		}
	}
}

QString TextDetector::Text()
{
	return m_resultText;
}
