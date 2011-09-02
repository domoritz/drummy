#include "previewdialog.h"
#include "ui_previewdialog.h"
#include <QDebug>
#import <QFont>
#include <QTextCharFormat>

PreviewDialog::PreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreviewDialog)
{
    ui->setupUi(this);
}

PreviewDialog::~PreviewDialog()
{
    delete ui;
}

void PreviewDialog::setText(QString text)
{
    ui->textEdit->setText(text);
}

void PreviewDialog::readFont()
{
    #ifdef Q_WS_MACX
    QString family = "Monaco";
    int size = 13;
    #else
    // non Mac code here
    QString family = "Courier"
    int size = 13;
    #endif

    QFont font = QFont(settings.value("font/family",family).toString(),settings.value("font/size",size).toInt());

    ui->textEdit->setFont(font);
}
