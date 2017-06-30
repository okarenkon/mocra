#ifndef DETECTDIALOG_H
#define DETECTDIALOG_H

#include <QDialog>
#include "ui_DetectDialog.h"

class DetectDialog : public QDialog, private Ui::DetectDialog
{
    Q_OBJECT

public:
    DetectDialog(QWidget *parent = 0);

	void setCredentialsFilePath(const QString& credentialsFilePath);
	QString credentialsFilePath();

private slots:
	void onBrowseFile();
};

#endif
