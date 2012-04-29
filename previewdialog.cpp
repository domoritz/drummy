/**
 * Preview widget
 * ------------
 * Shows a preview of the text output. Lets the user print it if he/she wants to.
 */


#include "previewdialog.h"
#include "ui_previewdialog.h"
#include <QDebug>
#include <QFont>
#include <QTextCharFormat>
#include <QPrinter>
#include <QPrintDialog>

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
    // default
    #ifdef Q_WS_MACX
    QString family = "Monaco";
    int size = 13;
    #else
    // non Mac code here
    QString family = "Courier";
    int size = 13;
    #endif

    // get font from settings
    QFont font = QFont(settings.value("font/family",family).toString(),settings.value("font/size",size).toInt());

    ui->textEdit->setFont(font);
}

void PreviewDialog::on_actionPrint_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (ui->textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Tabs"));
    if (dlg->exec() == QDialog::Accepted) {
        ui->textEdit->print(&printer);
    }
    delete dlg;
}

void PreviewDialog::on_printPushButton_clicked()
{
    ui->actionPrint->trigger();
}
