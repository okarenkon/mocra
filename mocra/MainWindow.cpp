#include <QtWidgets>
#include "MainWindow.h"
#include "DetectDialog.h"
#include "TextDetector.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow()
   : m_imageLabel(new QLabel)
   , m_scrollArea(new QScrollArea)
   , m_textedit(new QTextEdit)
   , m_textlog(new QTextEdit)
   , m_scaleFactor(1)
   , m_imgPages()
   , m_textPages()
   , m_nowPage(0)
   , m_credentialsFilePath()
   , m_enableImageCollect(true)
   , m_enableGrayScale(true)
   , m_enableDenoise(true)
   , m_enableSlantCorrect(true)
   , m_enableDetectLines(true)
{
    m_imageLabel->setBackgroundRole(QPalette::Base);
    m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imageLabel->setScaledContents(true);

    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_imageLabel);
    m_scrollArea->setVisible(true);

	m_textedit->setBackgroundRole(QPalette::Base);
	m_textedit->setVisible(true);

	QSplitter *splitter = new QSplitter(this);
	splitter->addWidget(m_scrollArea);
	splitter->addWidget(m_textedit);
	splitter->setSizes(QList<int>() << 1 << 1);
	
	setCentralWidget(splitter);

	m_textlog->setBackgroundRole(QPalette::Base);
	m_textlog->setVisible(true);

	QDockWidget *dock = new QDockWidget(tr("出力"), this);
	dock->setAllowedAreas(Qt::BottomDockWidgetArea);
	dock->setWidget(m_textlog);
	addDockWidget(Qt::BottomDockWidgetArea, dock);

	statusBar()->setVisible(true);

    createActions();

	if (! readSettings()) {
		resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
	}
}

MainWindow::~MainWindow()
{
	writeSettings();
}


bool MainWindow::loadFile(const QString &fileName)
{
	MultiPageImageReader mpi(std::string(fileName.toLocal8Bit().data()));

	for (;;) {
		bool eof = false;

		//画像を1ページ読み込む
		cv::Mat img = mpi.read(&eof);
		if (img.empty()) {
			if (!eof) {
				return false;	//エラー
			}
			//最後まで読んでいたらループ抜ける
			break;
		}

		//cv::resize(img, img, cv::Size(768, 1024));
		//cv::namedWindow(fileName.toStdString(), CV_WINDOW_AUTOSIZE);
		//cv::imshow(fileName.toStdString(), img);
		//cv::waitKey(0);
		//cv::destroyWindow(fileName.toStdString());

		m_imgPages.append(img);
	}

	setImage(m_nowPage);
	fitToWindow();

    setWindowFilePath(fileName);

	const QString message = tr("画像ファイルを開きました \"%1\"").arg(QDir::toNativeSeparators(fileName));
	statusBar()->showMessage(message);

    return true;
}

void MainWindow::setImage(int page)
{
	if ((page >= 0)
	 && (page < m_imgPages.count())) {
		m_image = QImage(m_imgPages[page].ptr(), m_imgPages[page].cols, m_imgPages[page].rows, m_imgPages[page].step, QImage::Format_RGB888);
		m_imageLabel->setPixmap(QPixmap::fromImage(m_image));

		if ((page >= 0)
		 && (page < m_textPages.count())) {
			m_textedit->setText(m_textPages[page]);
		}
		else {
			m_textedit->clear();
		}

		m_scrollArea->setVisible(true);
		updateActions();
	}
}

bool MainWindow::saveFile(const QString &fileName)
{
	QFile textFile(fileName);
	if (textFile.open(QFile::WriteOnly | QFile::Truncate)) {
		QTextStream out(&textFile);
		for (int page = 0; page < m_textPages.count(); page++) {
			out << m_textPages[page];
		}

		const QString message = tr("テキストファイルを保存しました \"%1\"").arg(QDir::toNativeSeparators(fileName));
		statusBar()->showMessage(message);

		return true;
	}

	return false;
}

void MainWindow::open()
{
	QStringList filters;
	filters << 
		tr("すべての画像ファイル (*.png *.tif *.tiff *.bmp *.dib *.jpg *.jpeg *.jpe *.jp2)") <<
		tr("PNG ファイル (*.png)") <<
		tr("TIFF ファイル (*.tif *.tiff)") <<
		tr("ビットマップ ファイル (*.bmp *.dib)") <<
		tr("JPEG ファイル (*.jpg *.jpeg *.jpe)") <<
		tr("JPEG 2000 ファイル (*.jp2)") <<
		tr("すべてのファイル (*.*)");

	QFileDialog dialog(this, tr("開く"));
	dialog.setNameFilters(filters);
	if (dialog.exec() == QDialog::Accepted) {
		m_imgPages.clear();
		m_textPages.clear();
		if (! loadFile(dialog.selectedFiles().first())) {
			QMessageBox::warning(this, tr("mocra"), tr("画像ファイルを開けませんでした。\n") + "'" + dialog.selectedFiles().first() + "'");
		}
	}
}

void MainWindow::saveAs()
{
	QStringList filters;
	filters <<
		tr("テキスト ファイル (*.txt)") <<
		tr("すべてのファイル (*.*)");

    QFileDialog dialog(this, tr("名前を付けて保存"));
	dialog.setNameFilters(filters);
	if (dialog.exec() == QDialog::Accepted) {
		if (! saveFile(dialog.selectedFiles().first())) {
			QMessageBox::warning(this, tr("mocra"), tr("テキスト ファイルを保存できませんでした。\n") + "'" + dialog.selectedFiles().first() + "'");
		}
	}
}

void MainWindow::textDetect()
{
	DetectDialog dlg;
	dlg.editCredentialsFilePath->setText(m_credentialsFilePath);
	dlg.checkImageCollect->setCheckState(m_enableImageCollect ? Qt::Checked : Qt::Unchecked);
	dlg.checkGrayScale->setCheckState(m_enableGrayScale ? Qt::Checked : Qt::Unchecked);
	dlg.checkDenoise->setCheckState(m_enableDenoise ? Qt::Checked : Qt::Unchecked);
	dlg.checkSlantCorrect->setCheckState(m_enableSlantCorrect ? Qt::Checked : Qt::Unchecked);
	dlg.checkDetectLines->setCheckState(m_enableDetectLines ? Qt::Checked : Qt::Unchecked);
	if (dlg.exec() == QDialog::Accepted) {
		m_credentialsFilePath = dlg.editCredentialsFilePath->text();
		m_enableImageCollect = dlg.checkImageCollect->checkState() == Qt::Checked;
		m_enableGrayScale = dlg.checkGrayScale->checkState() == Qt::Checked;
		m_enableDenoise = dlg.checkDenoise->checkState() == Qt::Checked;
		m_enableSlantCorrect = dlg.checkSlantCorrect->checkState() == Qt::Checked;
		m_enableDetectLines = dlg.checkDetectLines->checkState() == Qt::Checked;

		m_textlog->clear();

		for (int page = 0; page < m_imgPages.count(); page++) {
			TextDetector detector(m_imgPages[page], m_credentialsFilePath, m_textlog);
			m_textPages.append(detector.Text());
		}

		m_textedit->setText(m_textPages[m_nowPage]);
	}
}

void MainWindow::nextPage()
{
	if (m_imgPages.count() > m_nowPage + 1) {
		m_nowPage++;
		setImage(m_nowPage);
	}
}

void MainWindow::prevPage()
{
	if (0 <= m_nowPage - 1) {
		m_nowPage--;
		setImage(m_nowPage);
	}
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    m_imageLabel->adjustSize();
    m_scaleFactor = 1.0;
}

void MainWindow::fitToWindow()
{
	normalSize();
	double scaleH = (double)m_scrollArea->width() / (double)m_imageLabel->width();
	double scaleV = (double)m_scrollArea->height() / (double)m_imageLabel->height();

	m_scaleFactor = (scaleH > scaleV ? scaleV : scaleH);
	scaleImage(1.0);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("mocra について"),
            tr("mocra version 0.1"));
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("ファイル(&F)"));

    QAction *openAct = fileMenu->addAction(tr("画像を開く(&O)..."), this, &MainWindow::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("名前を付けてテキストを保存(&A)..."), this, &MainWindow::saveAs);
    saveAsAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("終了(&X)"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("編集(&E)"));

	textDetectAct = editMenu->addAction(tr("テキスト認識(&T)"), this, &MainWindow::textDetect);
	textDetectAct->setEnabled(false);

    QMenu *viewMenu = menuBar()->addMenu(tr("表示(&V)"));

	nextPageAct = viewMenu->addAction(tr("次のページ(&N)"), this, &MainWindow::nextPage);
	nextPageAct->setEnabled(false);

	prevPageAct = viewMenu->addAction(tr("前のページ(&P)"), this, &MainWindow::prevPage);
	prevPageAct->setEnabled(false);

	viewMenu->addSeparator();
	
	zoomInAct = viewMenu->addAction(tr("ズーム イン(&I)"), this, &MainWindow::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("ズーム アウト(&O)"), this, &MainWindow::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("ピクセル等倍(&N)"), this, &MainWindow::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    fitToWindowAct = viewMenu->addAction(tr("全体を表示(&F)"), this, &MainWindow::fitToWindow);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));
	fitToWindowAct->setEnabled(false);

    QMenu *helpMenu = menuBar()->addMenu(tr("ヘルプ(&H)"));

    helpMenu->addAction(tr("mocra について(&A)..."), this, &MainWindow::about);
    helpMenu->addAction(tr("&Qt について..."), &QApplication::aboutQt);
}

void MainWindow::updateActions()
{
    saveAsAct->setEnabled(!m_image.isNull());
	textDetectAct->setEnabled(!m_image.isNull());
	nextPageAct->setEnabled(m_imgPages.count() > m_nowPage + 1);
	prevPageAct->setEnabled(0 <= m_nowPage - 1);
	zoomInAct->setEnabled(!m_image.isNull());
    zoomOutAct->setEnabled(!m_image.isNull());
    normalSizeAct->setEnabled(!m_image.isNull());
	fitToWindowAct->setEnabled(!m_image.isNull());
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(m_imageLabel->pixmap());
    m_scaleFactor *= factor;
    m_imageLabel->resize(m_scaleFactor * m_imageLabel->pixmap()->size());

    adjustScrollBar(m_scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(m_scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(m_scaleFactor < 3.0);
    zoomOutAct->setEnabled(m_scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(
		factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::writeSettings()
{
	QSettings settings(QApplication::applicationDirPath() + "/mocra.ini", QSettings::IniFormat);

	settings.beginGroup("MainWindow");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	settings.endGroup();

	settings.beginGroup("TextDetect");
	settings.setValue("credentialsFilePath",	m_credentialsFilePath);
	settings.setValue("enableImageCollect",		m_enableImageCollect);
	settings.setValue("enableGrayScale",		m_enableGrayScale);
	settings.setValue("enableDenoise",			m_enableDenoise);
	settings.setValue("enableSlantCorrect",		m_enableSlantCorrect);
	settings.setValue("enableDetectLines",		m_enableDetectLines);
	settings.endGroup();
}

bool MainWindow::readSettings()
{
	QSettings settings(QApplication::applicationDirPath() + "/mocra.ini", QSettings::IniFormat);

	settings.beginGroup("MainWindow");
	bool exists = settings.contains("geometry");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
	settings.endGroup();

	settings.beginGroup("TextDetect");
	m_credentialsFilePath	= settings.value("credentialsFilePath",	QString("")).toString();
	m_enableImageCollect	= settings.value("enableImageCollect",	true).toBool();
	m_enableGrayScale		= settings.value("enableGrayScale",		true).toBool();
	m_enableDenoise			= settings.value("enableDenoise",		true).toBool();
	m_enableSlantCorrect	= settings.value("enableSlantCorrect",	true).toBool();
	m_enableDetectLines		= settings.value("enableDetectLines",	true).toBool();
	settings.endGroup();

	return exists;
}
