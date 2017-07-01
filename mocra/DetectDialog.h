#ifndef DETECTDIALOG_H
#define DETECTDIALOG_H

#include <QDialog>
#include "ui_DetectDialog.h"

class DetectDialog : public QDialog, public Ui::DetectDialog
{
    Q_OBJECT
public:
    DetectDialog(QWidget *parent = 0);

private slots:
	void onBrowseFile();
	void onStateChangedCheckImageCollect(int state);
};

#endif
