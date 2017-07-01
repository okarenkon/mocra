#include <QtWidgets>
#include "DetectDialog.h"

#pragma execution_character_set("utf-8")

DetectDialog::DetectDialog(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);

	QObject::connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFile()));
	QObject::connect(checkImageCollect, SIGNAL(stateChanged(int)), this, SLOT(onStateChangedCheckImageCollect(int)));
}

void DetectDialog::onBrowseFile()
{
	QString fileName = editCredentialsFilePath->text();
	fileName = QFileDialog::getOpenFileName(this,
		tr("ファイル選択"), fileName, tr("JSON ファイル (*.json);;すべてのファイル (*.*)"));
	if (! fileName.isNull()) {
		editCredentialsFilePath->setText(QDir::toNativeSeparators(fileName));
	}
}

void DetectDialog::onStateChangedCheckImageCollect(int state)
{
	switch (state) {
	case Qt::Unchecked:
		checkGrayScale->setEnabled(false);
		checkDenoise->setEnabled(false);
		checkSlantCorrect->setEnabled(false);
		checkDetectLines->setEnabled(false);
		break;
	case Qt::Checked:
		checkGrayScale->setEnabled(false);
		checkDenoise->setEnabled(true);
		checkSlantCorrect->setEnabled(true);
		checkDetectLines->setEnabled(true);
		break;
	default:
		break;
	}
}
