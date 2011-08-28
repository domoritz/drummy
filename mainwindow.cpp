#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <QMessageBox>
#include "prefswindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setUnifiedTitleAndToolBarOnMac(true);
    settingsSet = false;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRecord_triggered()
{
    if (settingsSet) {
        this->statusBar()->showMessage(tr("Recoding started..."));
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
         } else if (prefPopup.clickedButton() == (QAbstractButton*)preferencesButton) {
             // prefs
         } else if (prefPopup.clickedButton() == (QAbstractButton*)cancelButton){
             // cancel
         }

    }
}


void MainWindow::on_actionPreferences_triggered()
{
    PrefsWindow *prefs = new PrefsWindow(this);
    prefs->show();
}
