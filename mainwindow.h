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
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    bool saveFile(const QString &fileName);

private:
    bool recording;
    PrefsWindow *prefs;
    void record();
    void stopRecording();
    QSystemTrayIcon *trayIcon;
    QSettings settings;
    QString curFile;

private slots:
    void on_actionSelect_All_triggered();
    void on_actionRecordingPreferences_triggered();
    void on_dockWidget_visibilityChanged(bool visible);
    void on_actionClear_triggered();
    void on_actionFullscreen_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionRecord_triggered();
    void on_actionPreferences_triggered();
    void closeEvent(QCloseEvent *event);
    void newFile();
    bool save();
    bool saveAs();
    void documentWasModified();
    void writeSettings();
    void readSettings();

public slots:
    void keyPressEvent ( QKeyEvent * event );
    void changeFont(QFont font);
    void progressTick();
    void changeBpm(int bpm);
    void startTimer();
};

#endif // MAINWINDOW_H
