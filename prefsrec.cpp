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
