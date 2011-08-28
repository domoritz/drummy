#include "prefswindow.h"
#include "ui_prefswindow.h"
#include <QIcon>

PrefsWindow::PrefsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrefsWindow)
{
    this->setUnifiedTitleAndToolBarOnMac(true);
    ui->setupUi(this);

    // set selected widget to application prefs
    ui->stackedWidget->setCurrentIndex(0);
    ui->actionApplication->setChecked(true);
    ui->actionRecording->setChecked(false);
}

PrefsWindow::~PrefsWindow()
{
    delete ui;
}

void PrefsWindow::on_actionApplication_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->actionApplication->setChecked(true);
    ui->actionRecording->setChecked(false);
}

void PrefsWindow::on_actionRecording_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->actionApplication->setChecked(false);
    ui->actionRecording->setChecked(true);
}
