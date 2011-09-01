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

    ui->actionPreferences->setEnabled(false);
    ui->actionAbout->setEnabled(false);

    // pass signals through
    connect(ui->app,SIGNAL(fontChanged(QFont)),this,SIGNAL(fontChanged(QFont)));
    connect(ui->rec,SIGNAL(bpmChanged(int)),this,SIGNAL(bpmChanged(int)));
    connect(ui->rec,SIGNAL(settingsChanged()),this,SIGNAL(settingsChanged()));
}

PrefsWindow::~PrefsWindow()
{
    delete ui;
}

void PrefsWindow::fe() {ui->app->fe();}

void PrefsWindow::show(int startIndex)
{
    // set selected widget to application prefs
    if (startIndex == 1) {
        ui->actionRecording->trigger();
    } else {
        ui->actionGeneral->trigger();
    }

    adjustSize();

    this->setVisible(true); // == show()
}

const QSizePolicy ignored(QSizePolicy::Ignored, QSizePolicy::Ignored);
const QSizePolicy preferred(QSizePolicy::Preferred, QSizePolicy::Preferred);
const QSizePolicy expanding(QSizePolicy::Expanding, QSizePolicy::Expanding);

void PrefsWindow::on_actionGeneral_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->app);
    ui->actionGeneral->setChecked(true);
    ui->actionRecording->setChecked(false);
    this->setWindowTitle(tr("Preferences"));
}

void PrefsWindow::on_actionRecording_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->rec);
    ui->actionGeneral->setChecked(false);
    ui->actionRecording->setChecked(true);

    ui->rec->reload();

    this->setWindowTitle(tr("Preferences - Recording"));
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
