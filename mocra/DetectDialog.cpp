#include <QtWidgets>
#include "DetectDialog.h"

#pragma execution_character_set("utf-8")

DetectDialog::DetectDialog(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);

	QObject::connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFile()));
}

void DetectDialog::setCredentialsFilePath(const QString& credentialsFilePath)
{
	editCredentialsFilePath->setText(credentialsFilePath);
}

QString DetectDialog::credentialsFilePath()
{
	return editCredentialsFilePath->text();
}

void DetectDialog::onBrowseFile()
{
	QString fileName = editCredentialsFilePath->text();
	fileName = QFileDialog::getOpenFileName(this,
		tr("�t�@�C���I��"), fileName, tr("JSON �t�@�C�� (*.json);;���ׂẴt�@�C�� (*.*)"));
	if (! fileName.isNull()) {
		editCredentialsFilePath->setText(QDir::toNativeSeparators(fileName));
	}
}
