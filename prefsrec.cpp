#include "prefsrec.h"
#include "ui_prefsrec.h"

PrefsRec::PrefsRec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefsRec)
{
    ui->setupUi(this);
}

PrefsRec::~PrefsRec()
{
    delete ui;
}

/* someone changed the value of the slider */
void PrefsRec::on_bpmHorizontalSlider_valueChanged(int value)
{
    bpm = ui->bpmHorizontalSlider->value();
    ui->bpmLineEdit->setText(QString::number(bpm));
}

void PrefsRec::on_bpmLineEdit_editingFinished()
{
    bpm = ui->bpmLineEdit->text().toInt();
    ui->bpmHorizontalSlider->setValue(bpm);
}
