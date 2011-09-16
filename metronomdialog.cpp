#include "metronomdialog.h"
#include "ui_metronomdialog.h"
#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>

MetronomDialog::MetronomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MetronomDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    // every second
    timer.setInterval(10);
    connect(&timer,SIGNAL(timeout()),this,SLOT(refreshTime()));

    setResult(0);
}

MetronomDialog::~MetronomDialog()
{
    delete ui;
}

void MetronomDialog::keyPressEvent ( QKeyEvent * event ){
    int remaining;

    remaining =  ui->remainingLabel->text().toInt();
    remaining--;

    if (remaining == 7) {
        timer.start();
        time.start();
    }

    if (remaining >= 0) {
        lastTimeInMsec = time.elapsed();
        ui->remainingLabel->setText(QString::number(remaining));

        if (lastTimeInMsec > 0) {
            bpm = (8-remaining)*60*1000/lastTimeInMsec;
            ui->averageLabel->setText("<strong>"+QString::number(bpm)+"</strong>");

            if (bpm < 40 || 280 < bpm) {
                // bpm out of range
                ui->averageLabel->setText("<strong style='color:red'>"+QString::number(bpm)+"</strong> "+tr("(out of range)"));
            }
        }
    }

    if (remaining == 0) {
        timer.stop();
        ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled(true);
        if (40 < bpm && bpm < 280) {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    }
}

void MetronomDialog::refreshTime()
{
    QTime tmp(0,0,0);
    tmp = tmp.addMSecs(time.elapsed());
    ui->elapsedLabel->setText(tmp.toString("mm:ss:zzz"));
}

int MetronomDialog::getBpm()
{
    return bpm;
}

void MetronomDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->button(QDialogButtonBox::Reset) == button) {
        ui->averageLabel->setText("<strong>0</strong>");
        ui->elapsedLabel->setText("00:00:000");
        ui->remainingLabel->setText("8");

        ui->buttonBox->button(QDialogButtonBox::Reset)->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
