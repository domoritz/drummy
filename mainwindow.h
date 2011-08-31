#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "prefswindow.h"
#include <QtDebug>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QtGui>
#include "painter.h"
#include <QTimer>
#include <QFont>

class Assistant;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    QProgressBar *progressBar;

protected:
    Painter painter;
    QTimer *timer;

private:
    bool recording;
    PrefsWindow *prefs;
    void record();
    void stopRecording();
    QSystemTrayIcon *trayIcon;
    QSettings settings;

private slots:
    void on_actionClear_triggered();
    void on_actionActionRecordingPreferences_triggered();
    void on_actionFullscreen_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionPreferences_triggered();
    void on_actionRecord_triggered();
    void keyPressEvent ( QKeyEvent * event );
    void changeFont(QFont font);
};

#endif // MAINWINDOW_H
