#include <QApplication>
#include <QTextCodec>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift-JIS"));
	QGuiApplication::setApplicationDisplayName(QObject::tr("mocra"));
	MainWindow mainWindow;
	mainWindow.show();
    return app.exec();
}
