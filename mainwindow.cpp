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

    /* use fe to force emit of font change
     problem was:
        i cannot connect signals and slots before the ui is initalized because i need the objects. however, if i do so, the emit from the
        constructor does not go down here because no connection is established. qt does not have an opposite thing of emit which foreces a refresh!!!
    */
    prefs->fe();
}

MainWindow::~MainWindow()
{
    delete timer;
    delete progressTimer;
    delete trayIcon;
    delete ui;
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
    recording = true;
    this->setFocus();
    //ui->textEdit->setEnabled(false);
    //ui->textEdit->setFocusPolicy(Qt::NoFocus);

    //inform user
    trayIcon->show();
    this->trayIcon->showMessage(tr("Recording"),tr("Recoding started. Use selected keys."),QSystemTrayIcon::NoIcon,1000);

    // change action icon to indicate recording
    ui->actionRecord->setIcon(QIcon(":/images/record_32.png"));

    // initalize textedit
    painter.init();

    //start timer
    timer->start(1000*60/settings.value("bpm",120).toInt());
    if(settings.value("progress",true).toBool()) {
        progressBar->show();
        progressTimer->start(10*60/settings.value("bpm",120).toInt());
    }
}

void MainWindow::stopRecording() {
    recording = false;
    ui->textEdit->setEnabled(true);
    //ui->textEdit->setFocusPolicy(Qt::StrongFocus);

    timer->stop();

    this->trayIcon->showMessage(tr("Finished recording"),tr("Recoding stopped."),QSystemTrayIcon::NoIcon,1000);

    ui->actionRecord->setIcon(QIcon(":/images/record_off_32.png"));

    progressBar->hide();
}

// updates the progressbar
void MainWindow::progressTick()
{
    progressBar->setValue(counter);
    counter+=1;
    counter%=100;
}

void MainWindow::on_actionPreferences_triggered()
{
    prefs->setWindowModality(Qt::ApplicationModal);
    prefs->show(0);
}

void MainWindow::on_actionActionRecordingPreferences_triggered()
{
    prefs->setWindowModality(Qt::ApplicationModal);
    prefs->show(1);
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
}

void MainWindow::changeFont(QFont font)
{
    qDebug() << "font changed to" << font.family() << font.pointSize();
    ui->textEdit->setFont(font);
};
