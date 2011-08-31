#include "prefswindow.h"
#include "ui_prefswindow.h"
#include <QIcon>
#include "mainwindow.h"

PrefsWindow::PrefsWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::PrefsWindow)
{
    this->setUnifiedTitleAndToolBarOnMac(true);

     ui->setupUi(this);

    // pass font signal through
    connect(ui->app,SIGNAL(fontChanged(QFont)),this,SIGNAL(fontChanged(QFont)));
}

PrefsWindow::~PrefsWindow()
{
    delete ui;
}

void PrefsWindow::fe() {ui->app->fe();}

void PrefsWindow::show(int startIndex)
{
    // set selected widget to application prefs
    ui->stackedWidget->setCurrentIndex(startIndex);
    ui->actionGeneral->setChecked(!startIndex);
    ui->actionRecording->setChecked(startIndex);

    ui->actionPreferences->setEnabled(false);
    ui->actionAbout->setEnabled(false);

    adjustSize();

    this->setVisible(true); // == show()
}

const QSizePolicy ignored(QSizePolicy::Ignored, QSizePolicy::Ignored);
const QSizePolicy preferred(QSizePolicy::Preferred, QSizePolicy::Preferred);
const QSizePolicy expanding(QSizePolicy::Expanding, QSizePolicy::Expanding);


void PrefsWindow::on_actionRecording_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->rec);
    ui->actionGeneral->setChecked(false);
    ui->actionRecording->setChecked(true);
}

void PrefsWindow::on_stackedWidget_currentChanged(int index)
{
    /*if (index == 0) {
        ui->stackedWidget->widget(0)->setSizePolicy(ignored);
        ui->stackedWidget->widget(1)->setSizePolicy(ignored);
        statusBar()->showMessage("0");
    } else {
        ui->stackedWidget->widget(1)->setSizePolicy(ignored);
        ui->stackedWidget->widget(0)->setSizePolicy(ignored);
        statusBar()->showMessage("1");
    }
    */
    //layout()->activate();
    //adjustSize();
}

void PrefsWindow::on_actionGeneral_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->app);
    ui->actionGeneral->setChecked(true);
    ui->actionRecording->setChecked(false);
}
