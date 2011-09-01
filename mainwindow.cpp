#include "mainwindow.h"
#include "about.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "helpdialog.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    // unify the title and toolbar on mac
    this->setUnifiedTitleAndToolBarOnMac(true);

    recording = false;

    this->setAnimated(true);

    ui->setupUi(this);

    // preferences window
    prefs = new PrefsWindow();

    trayIcon = new QSystemTrayIcon(this);

    // connect font change signal/slot
    connect(prefs, SIGNAL(fontChanged(QFont)),this,SLOT(changeFont(QFont)));

    // quit-connection
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    //give textedit pointer to painter
    painter.setTextEdit(ui->textEdit);

    //create progress bar
    progressBar = new QProgressBar();
    ui->statusBar->addWidget(progressBar,1);
    progressBar->hide();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), &painter, SLOT(tick()));

    progressTimer = new QTimer(this);
    connect(progressTimer, SIGNAL(timeout()), this, SLOT(progressTick()));

    // connect bpm change
    connect(ui->dockWidgetContents,SIGNAL(bpmChanged(int)),this,SLOT(changeBpm(int)));
    connect(prefs,SIGNAL(bpmChanged(int)),this,SLOT(changeBpm(int)));

    // connect settings changed
    connect(prefs,SIGNAL(settingsChanged()),ui->dockWidgetContents,SLOT(reload()));

    /* use fe to force emit of font change
     problem was:
        i cannot connect signals and slots before the ui is initalized because i need the objects. however, if i do so, the emit from the
        constructor does not go down here because no connection is established. qt does not have an opposite thing of emit which foreces a refresh!!!
    */
    prefs->fe();

    #ifdef Q_WS_MAC
    ui->dockWidget->setWindowFlags(Qt::Drawer);
    this->addDockWidget(Qt::RightDockWidgetArea,ui->dockWidget);
    #endif

    ui->dockWidget->hide();

}

MainWindow::~MainWindow()
{
    delete timer;
    delete progressTimer;
    delete trayIcon;
    delete ui;
}

// fix issue with drawer when closing app
// https://bugreports.qt.nokia.com//browse/QTBUG-15897
void MainWindow::closeEvent(QCloseEvent *event)
{
    this->setFocus();
    QWidget::close();
}

void MainWindow::on_actionRecord_triggered()
{
    if (recording) {
        stopRecording();
    } else {
        // if the mapping size is set and bigger than 0 we are ready to record
        bool settingsSet = !settings.value("mappings/size").isNull() && settings.value("mappings/size").toInt() > 0;

        if (settingsSet) {
            record();
        } else {
            QMessageBox prefPopup(this);
            prefPopup.setModal(true);
            prefPopup.setWindowModality(Qt::WindowModal);
            //prefPopup.setIcon(QMessageBox::Question);
            prefPopup.setText(tr("No recoding Preferences!"));
            prefPopup.setInformativeText(tr("You haven't set your recording preferences. Do you want to use your own or use the default preferences?"));

            QPushButton *preferencesButton = prefPopup.addButton(tr("Set preferences"), QMessageBox::ActionRole);
            QPushButton *defaultButton = prefPopup.addButton(tr("Use defaults"), QMessageBox::ActionRole);
            QPushButton *cancelButton = prefPopup.addButton(QMessageBox::Cancel);

            prefPopup.setDefaultButton(preferencesButton);
            prefPopup.exec();

            if (prefPopup.clickedButton() == (QAbstractButton*)defaultButton) {
                // default

                //TODO set defaults
                record();
            } else if (prefPopup.clickedButton() == (QAbstractButton*)preferencesButton) {
                // prefs
                prefs->setWindowModality(Qt::ApplicationModal);
                prefs->show(1);
            } else if (prefPopup.clickedButton() == (QAbstractButton*)cancelButton){
                // cancel
            }

        }
    }
}

void MainWindow::record() {
    //inform user
    trayIcon->show();
    this->trayIcon->showMessage(tr("Recording"),tr("Recoding started. Use selected keys."),QSystemTrayIcon::NoIcon,1000);

    // change action icon to indicate recording
    ui->actionRecord->setIcon(QIcon(":/images/record_32.png"));

    // initalize textedit
    painter.init();

    // disable some widgets
    this->setFocus();
    ui->textEdit->setEnabled(false);
    //ui->textEdit->setFocusPolicy(Qt::NoFocus);

    ui->actionClear->setEnabled(false);
    ui->actionPreferences->setEnabled(false);

    ui->dockWidgetContents->setFullyEnabled(false);

    //start timer
    // hint: if the progress timer is enabled it will emit the tick to the painter
    // this makes it stay in sync
    if(settings.value("progress",true).toBool()) {
        progressBar->show();
        progressTimer->start(10*60/settings.value("bpm",120).toInt());
    } else {
        timer->start(1000*60/settings.value("bpm",120).toInt());
    }

    recording = true;
}

void MainWindow::stopRecording() {
    timer->stop();
    progressTimer->stop();

    recording = false;

    // reenable some widgets
    ui->textEdit->setEnabled(true);
    //ui->textEdit->setFocusPolicy(Qt::StrongFocus);

    ui->actionClear->setEnabled(true);

    ui->dockWidgetContents->setFullyEnabled(true,true);

    this->trayIcon->showMessage(tr("Finished recording"),tr("Recoding stopped."),QSystemTrayIcon::NoIcon,1000);

    ui->actionRecord->setIcon(QIcon(":/images/record_off_32.png"));

    progressBar->hide();
}

// updates the progressbar
void MainWindow::progressTick()
{
    progressBar->setValue(counter);
    counter+=1;
    if (counter%100 == 0) {
        counter = 0;
        painter.tick();
    }
}

// if bpm changed use this slot
void MainWindow::changeBpm(int bpm)
{
    timer->setInterval(1000*60/bpm);
    progressTimer->setInterval(10*60/bpm);

    ui->dockWidgetContents->reload();
}

void MainWindow::on_actionPreferences_triggered()
{
    prefs->setWindowModality(Qt::ApplicationModal);
    prefs->show(0);
}

void MainWindow::on_actionRecordingPreferences_triggered()
{
    //prefs->setWindowModality(Qt::ApplicationModal);
    //prefs->show(1);

    if (ui->dockWidget->isVisible()){
        ui->dockWidget->hide();
    } else {
        ui->dockWidget->show();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    About *aw = new About(this);
    aw->setWindowModality(Qt::ApplicationModal);
    aw->show();
}

void MainWindow::on_actionHelp_triggered()
{
    HelpDialog *hw = new HelpDialog(this);
    hw->setWindowModality(Qt::ApplicationModal);
    hw->show();
}

void MainWindow::on_actionFullscreen_triggered()
{
    if (this->isFullScreen()){
        this->showNormal();
        ui->actionFullscreen->setText(tr("Fullscreen"));
    } else {
        this->showFullScreen();
        ui->actionFullscreen->setText(tr("Quit Fullsceen"));
    }
}

void MainWindow::keyPressEvent ( QKeyEvent * event ){
    if (recording) {
        // avoid repeating keys
        if(!event->isAutoRepeat()){
            painter.keyPressed(event);
        }
    }
}

void MainWindow::on_actionClear_triggered()
{
    ui->textEdit->clear();
    ui->actionPreferences->setEnabled(true);
    ui->dockWidgetContents->setFullyEnabled(true,false);
}

void MainWindow::changeFont(QFont font)
{
    qDebug() << "font changed to" << font.family() << font.pointSize();
    ui->textEdit->setFont(font);
};

void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{
    ui->actionRecordingPreferences->setChecked(visible);
}
