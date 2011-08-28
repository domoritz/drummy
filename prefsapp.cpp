#include "prefsapp.h"
#include "ui_prefsapp.h"

PrefsApp::PrefsApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefsApp)
{
    ui->setupUi(this);
}

PrefsApp::~PrefsApp()
{
    delete ui;
}
