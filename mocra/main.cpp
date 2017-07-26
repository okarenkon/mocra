#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QGuiApplication::setApplicationDisplayName(QObject::tr("mocra"));
	MainWindow mainWindow;
	mainWindow.show();
    return app.exec();
}
