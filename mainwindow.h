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
#include <QIcon>
#include <QCloseEvent>
#include <QUndoCommand>
#include <QUndoStack>
#include <QUndoView>
#include "previewdialog.h"
#include "mappings.h"

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
    QUndoStack *undoStack;

protected:
    Painter painter;
    QTimer *timer;
    QTimer *progressTimer;
    int counter;
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    QProgressBar *progressBar;

private:
    bool recording;
    PrefsWindow *prefs;
    void record();
    void stopRecording();
    QSystemTrayIcon *trayIcon;
    QSettings settings;
    QString curFile;
    QUndoView *undoView;
    PreviewDialog *previewDialog;
    QFileDialog *saveDialog;
    bool quit;
    Mappings map;

private slots:
    void on_actionPrint_triggered();
    void on_actionPreview_triggered();
    void on_actionSelect_All_triggered();
    void on_actionRecordingPreferences_triggered();
    void on_recDockWidget_visibilityChanged(bool visible);
    void on_actionClear_triggered();
    void on_actionFullscreen_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionRecord_triggered();
    void on_actionPreferences_triggered();
    void closeEvent(QCloseEvent *event);
    void newFile();
    void open();
    void save();
    void saveAs();
    bool maybeSave();
    void documentWasModified();
    bool mappingsAreDeclared();
    void setAndEnableShortcuts();
    void writeSettings();
    void readSettings();
    QString buildOutput();
    void saveDialogAccepted();
    void on_actionSongInformation_triggered();
    void on_songinfoDockWidget_visibilityChanged(bool visible);
    void on_actionStatusBar_triggered();

    void on_actionToolbar_triggered();

public slots:
    void keyPressEvent ( QKeyEvent * event );
    void changeFont(QFont font);
    void progressTick();
    void changeBpm(int bpm);
    void startTimer();
};

#endif // MAINWINDOW_H
