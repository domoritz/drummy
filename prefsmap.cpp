#include "prefsmap.h"
#include "ui_prefsmap.h"

PrefsMap::PrefsMap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefsMap)
{
    ui->setupUi(this);
}

PrefsMap::~PrefsMap()
{
    delete ui;
}
