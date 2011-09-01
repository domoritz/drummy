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
#include <QCloseEvent>

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
    QTimer *progressTimer;
    int counter;

private:
    bool recording;
    PrefsWindow *prefs;
    void record();
    void stopRecording();
    QSystemTrayIcon *trayIcon;
    QSettings settings;

private slots:
    void on_actionRecordingPreferences_triggered();
    void on_dockWidget_visibilityChanged(bool visible);
    void on_actionClear_triggered();
    void on_actionFullscreen_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionRecord_triggered();
    void on_actionPreferences_triggered();
    void closeEvent(QCloseEvent *event);

public slots:
    void keyPressEvent ( QKeyEvent * event );
    void changeFont(QFont font);
    void progressTick();
    void changeBpm(int bpm);
};

#endif // MAINWINDOW_H
