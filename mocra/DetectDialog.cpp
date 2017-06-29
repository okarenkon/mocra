#include <QtWidgets>

#include "DetectDialog.h"

DetectDialog::DetectDialog(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);
}

void DetectDialog::setCredentialsFilePath(const QString& credentialsFilePath)
{
	editCredentialsFilePath->setText(credentialsFilePath);
}

QString DetectDialog::credentialsFilePath()
{
	return editCredentialsFilePath->text();
}
