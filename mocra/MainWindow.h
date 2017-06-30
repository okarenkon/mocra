#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "MultiPageImageReader.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	bool loadFile(const QString &);

private slots:
    void open();
    void saveAs();
	void textDetect();
	void nextPage();
	void prevPage();
	void zoomIn();
	void zoomOut();
	void normalSize();
    void fitToWindow();
    void about();

private:
    void createActions();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(int page);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void writeSettings();
	bool readSettings();

    QImage			m_image;
    QLabel*			m_imageLabel;
    QScrollArea*	m_scrollArea;
	QTextEdit*		m_textedit;
	QTextEdit*		m_textlog;
	double			m_scaleFactor;
	QVector<cv::Mat> m_imgPages;
	QVector<QString> m_textPages;
	int				m_nowPage;
	QString			m_credentialsFilePath;

    QAction *saveAsAct;
	QAction *textDetectAct;
	QAction *nextPageAct;
	QAction *prevPageAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
};

#endif