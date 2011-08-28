#include "prefswindow.h"
#include "ui_prefswindow.h"
#include <QIcon>

PrefsWindow::PrefsWindow(QWidget *parent, int startIndex) :
        QMainWindow(parent),
        ui(new Ui::PrefsWindow)
{
    this->setUnifiedTitleAndToolBarOnMac(true);
    ui->setupUi(this);

    // set selected widget to application prefs
    ui->stackedWidget->setCurrentIndex(startIndex);
    ui->actionApplication->setChecked(!startIndex);
    ui->actionRecording->setChecked(startIndex);

    adjustSize();
}

PrefsWindow::~PrefsWindow()
{
    delete ui;
}

const QSizePolicy ignored(QSizePolicy::Ignored, QSizePolicy::Ignored);
const QSizePolicy preferred(QSizePolicy::Preferred, QSizePolicy::Preferred);
const QSizePolicy expanding(QSizePolicy::Expanding, QSizePolicy::Expanding);

void PrefsWindow::on_actionApplication_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->app);
    ui->actionApplication->setChecked(true);
    ui->actionRecording->setChecked(false);
}

void PrefsWindow::on_actionRecording_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->rec);
    ui->actionApplication->setChecked(false);
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
