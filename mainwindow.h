#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prefswindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool settingsSet;
    PrefsWindow *prefs;

private slots:
    void on_actionPreferences_triggered();
    void on_actionRecord_triggered();
};

#endif // MAINWINDOW_H
