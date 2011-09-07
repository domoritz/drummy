#include "mainwindow.h"
#include "about.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "helpdialog.h"
#include <QKeyEvent>
#include "songinfo.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    // unify the title and toolbar on mac
    this->setUnifiedTitleAndToolBarOnMac(true);

    recording = false;

    this->setAnimated(true);

    ui->setupUi(this);

#ifdef Q_WS_MAC
    // show dockwidget as drawer on a mac
    ui->dockWidget->setWindowFlags(Qt::Drawer);
    this->addDockWidget(Qt::RightDockWidgetArea,ui->dockWidget);
#endif

    readSettings();

    //shortcuts, platform independant
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionCut->setShortcut(QKeySequence::Cut);
    ui->actionDelete->setShortcut(QKeySequence::Delete);
    ui->actionHelp->setShortcut(QKeySequence::HelpContents);
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionPaste->setShortcut(QKeySequence::Paste);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSave_As->setShortcut(QKeySequence::SaveAs);
    ui->actionPreferences->setShortcut(QKeySequence::Preferences);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionSelect_All->setShortcut(QKeySequence::SelectAll);
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionPrint->setShortcut(QKeySequence::Print);


    // simple undo/ redo
    connect(ui->tabsTextEdit->document(), SIGNAL(undoAvailable(bool)),
            ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->tabsTextEdit->document(), SIGNAL(redoAvailable(bool)),
            ui->actionRedo, SLOT(setEnabled(bool)));

    setWindowModified(ui->tabsTextEdit->document()->isModified());
    ui->actionUndo->setEnabled(ui->tabsTextEdit->document()->isUndoAvailable());
    ui->actionRedo->setEnabled(ui->tabsTextEdit->document()->isRedoAvailable());

    connect(ui->actionUndo, SIGNAL(triggered()), ui->tabsTextEdit, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), ui->tabsTextEdit, SLOT(redo()));

    // preferences window
    prefs = new PrefsWindow();

    trayIcon = new QSystemTrayIcon(this);

    // preview
    previewDialog = new PreviewDialog(this);
    previewDialog->setWindowModality(Qt::WindowModal);

    // connect font change signal/slot
    connect(prefs, SIGNAL(fontChanged(QFont)),this,SLOT(changeFont(QFont)));

    // quit-connection
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    //give textedit pointer to painter
    painter.setTextEdit(ui->tabsTextEdit);

    //create progress bar
    progressBar = new QProgressBar();
    ui->statusBar->addWidget(progressBar,1);
    progressBar->hide();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), &painter, SLOT(tick()));

    progressTimer = new QTimer(this);
    connect(progressTimer, SIGNAL(timeout()), this, SLOT(progressTick()));

    // document
    connect(ui->tabsTextEdit->document(), SIGNAL(contentsChanged()),this, SLOT(documentWasModified()));
    setCurrentFile("");

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    connect(ui->tabsTextEdit, SIGNAL(copyAvailable(bool)),
            ui->actionCut, SLOT(setEnabled(bool)));
    connect(ui->tabsTextEdit, SIGNAL(copyAvailable(bool)),
            ui->actionCopy, SLOT(setEnabled(bool)));

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAs()));

    // connect bpm change
    connect(ui->dockWidgetContents,SIGNAL(bpmChanged(int)),this,SLOT(changeBpm(int)));
    connect(prefs,SIGNAL(bpmChanged(int)),this,SLOT(changeBpm(int)));

    // connect settings changed
    connect(prefs,SIGNAL(settingsChanged()),ui->dockWidgetContents,SLOT(reload()));

    // save(as)/ export dialog
    saveDialog = new QFileDialog(this);
    saveDialog->setWindowModality(Qt::WindowModal);
    saveDialog->setFilter(QDir::Files);
    saveDialog->setFileMode(QFileDialog::AnyFile);
    saveDialog->setAcceptMode(QFileDialog::AcceptSave);
    saveDialog->setViewMode(QFileDialog::List);
    connect(saveDialog,SIGNAL(accepted()),this,SLOT(saveDialogAccepted()));
    quit = false;

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


void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();

    quit = true;

    if (maybeSave()) {
        // fix issue with drawer when closing app
        // https://bugreports.qt.nokia.com//browse/QTBUG-15897
        this->setFocus();
        event->accept();
    } else {
        event->ignore();
    }

    QWidget::close();
}

void MainWindow::writeSettings()
{
    settings.setValue("window/pos", pos());
    settings.setValue("window/size", size());
    settings.setValue("window/recprefs", ui->dockWidget->isVisible());
    //settings.setValue("window/songinfo", ui->songInfoWidget->isVisible());
}

void MainWindow::readSettings()
{
    QPoint pos = settings.value("window/pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("window/size", QSize(800, 500)).toSize();
    ui->dockWidget->setVisible(settings.value("window/recprefs",false).toBool());
    //ui->songInfoWidget->setVisible(settings.value("window/songinfo",true).toBool());
    resize(size);
    move(pos);
}

bool MainWindow::maybeSave()
{
    if (this->isWindowModified()) {
        QMessageBox saveMsgBox(this);

        saveMsgBox.setText("The document has been modified.");
        saveMsgBox.setInformativeText("Do you want to save your changes?");
        saveMsgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        saveMsgBox.setDefaultButton(QMessageBox::Save);
        saveMsgBox.setWindowModality(Qt::WindowModal);
        int ret = saveMsgBox.exec();

        if (ret == QMessageBox::Save) {
            save();
            return false;
        } else if (ret == QMessageBox::Cancel) {
            quit = false;
            return false;
        }
    }
    return true;
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        ui->tabsTextEdit->clear();
        setCurrentFile("");
        // toDo delete song infos
    }
}

void MainWindow::save()
{
    if (curFile.isEmpty()) {
        saveAs();
    } else {
        saveFile(curFile);
    }
}

void MainWindow::saveAs()
{
    saveDialog->setWindowTitle(tr("Save tabs"));
    saveDialog->setDefaultSuffix("txt");
    saveDialog->setDirectory(QDir::homePath());
    saveDialog->show();
}

void MainWindow::saveDialogAccepted()
{
    QString fileName;
    fileName = saveDialog->selectedFiles()[0];

    qDebug() << fileName.isEmpty();
    if (fileName.isEmpty()) {
        // no file
        // should never be called because this slot is only called on accept
    } else {
        saveFile(fileName);
    }
    if (quit)
        this->close();

}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox warn(this);
        warn.setIcon(QMessageBox::Warning);
        warn.setText(tr("Cannot write file %1:\n%2.")
                     .arg(fileName)
                     .arg(file.errorString()));
        warn.setWindowModality(Qt::ApplicationModal);
        warn.exec();

        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // ToDo: add song/tab information
    out << buildOutput();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    //ui->statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::documentWasModified()
{
    setWindowModified(ui->tabsTextEdit->document()->isModified());
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    ui->tabsTextEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowFilePath(shownName);

#ifndef Q_WS_MAC
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Drummy")));
#endif
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
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
            //prefPopup.setDetailedText(tr("The default settings are drum mappings and look like this: \nHh - High-hat\nT  - Low tom\nCc - Crash"));

            QPushButton *preferencesButton = prefPopup.addButton(tr("Set preferences"), QMessageBox::ActionRole);
            QPushButton *defaultButton = prefPopup.addButton(tr("Use defaults"), QMessageBox::ActionRole);
            QPushButton *cancelButton = prefPopup.addButton(QMessageBox::Cancel);

            prefPopup.setDefaultButton(preferencesButton);
            prefPopup.exec();

            if (prefPopup.clickedButton() == (QAbstractButton*)defaultButton) {
                // default
                prefs->setMappingDefaults();
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
    // variable indicates that the user clicked recording.
    // it helps to detect if the user cancelled recording while it is being initalized
    recording = true;

    //inform user
    trayIcon->show();
    this->trayIcon->showMessage(tr("Recording"),tr("Recoding started. Use selected keys."),QSystemTrayIcon::Information,1500);

    // change action icon to indicate recording
    ui->actionRecord->setIcon(QIcon(":/images/record_32.png"));

    // initalize textedit
    painter.init();

    // get focus to catch keyevents
    this->setFocus();

    // disable some widgets
    ui->tabsTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);

    ui->songInfoWidget->setDisabled(true);
    ui->actionClear->setEnabled(false);
    ui->actionPreferences->setEnabled(false);
    ui->actionPreview->setEnabled(false);

    // disable menu to avod strange behavior
    ui->menuBar->setEnabled(false);

    ui->dockWidgetContents->setFullyEnabled(false);

    // set progressbar to intermedia to indicate that recording will be started soon
    if(settings.value("progress",true).toBool()) {
        progressBar->show();
        // set intermediate
        progressBar->setMaximum(0);
        progressBar->setValue(0);
    }

    //start timer with a short delay
    QTimer::singleShot(500,this,SLOT(startTimer()));
}

void MainWindow::startTimer()
{
    if (recording) {
        painter.tick();
        // hint: if the progress timer is enabled it will emit the tick to the painter
        // this makes it stay in sync
        changeBpm(settings.value("bpm",120).toInt());
        if(settings.value("progress",true).toBool()) {
            progressBar->setMaximum(100);
            progressTimer->start();
        } else {
            timer->start();
        }
    }
}

void MainWindow::stopRecording() {
    // stop timer
    timer->stop();
    progressTimer->stop();

    // remove last highlight
    painter.highlight(false);

    recording = false;

    // reenable some widgets
    ui->tabsTextEdit->setTextInteractionFlags(Qt::TextEditorInteraction);

    ui->songInfoWidget->setEnabled(true);
    ui->actionClear->setEnabled(true);
    ui->actionPreview->setEnabled(true);
    ui->dockWidgetContents->setFullyEnabled(true,true);

    ui->tabsTextEdit->setReadOnly(false);

    ui->menuBar->setEnabled(true);

    this->trayIcon->showMessage(tr("Finished recording"),tr("Recoding stopped."),QSystemTrayIcon::Information,1000);

    ui->actionRecord->setIcon(QIcon(":/images/record_off_32.png"));

    progressBar->hide();
}

// updates the progressbar
void MainWindow::progressTick()
{
    progressBar->setValue(counter);
    counter+=1;
    if (counter%25 == 0) {
        painter.tick();
        if (counter%100 == 0) {
            counter = 0;
        }
    }
}

QString MainWindow::buildOutput()
{
    QString out;
    out.append(ui->songInfoWidget->stringifyInfos());

    // mappings object
    Mappings map;

    out.replace("##mappings##",map.stringifyMappings());
    out.append("\n");
    out.append(ui->tabsTextEdit->toPlainText());
    return out;
}

// if bpm changed use this slot
void MainWindow::changeBpm(int bpm)
{    
    timer->setInterval(4*1000*60/bpm);
    progressTimer->setInterval(4*10*60/bpm);

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
        ui->actionFullscreen->setText(tr("Enter Fullscreen"));
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
    //ui->tabsTextEdit->clear();
    ui->tabsTextEdit->setReadOnly(true);
    ui->tabsTextEdit->setPlainText("");
    ui->tabsTextEdit->insertPlainText("");
    ui->actionPreferences->setEnabled(true);
    ui->dockWidgetContents->setFullyEnabled(true,false);
}

void MainWindow::changeFont(QFont font)
{
    qDebug() << "font changed to" << font.family() << font.pointSize();
    ui->tabsTextEdit->setFont(font);
};

void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{
    //qDebug() << "dock widget visibility changed";
    ui->actionRecordingPreferences->setChecked(visible);
}

void MainWindow::on_actionSelect_All_triggered()
{
    ui->tabsTextEdit->setFocus();
    ui->tabsTextEdit->selectAll();
}

// show preview of output
void MainWindow::on_actionPreview_triggered()
{
    QString text;
    text.append(buildOutput());

    previewDialog->setText(text);
    previewDialog->readFont();
    previewDialog->show();
}

void MainWindow::on_actionPrint_triggered()
{
    QString text;
    text.append(buildOutput());
    previewDialog->setText(text);
    previewDialog->readFont();

    previewDialog->on_actionPrint_triggered();
}

