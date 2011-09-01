#ifndef PREFSWINDOW_H
#define PREFSWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QFont>

namespace Ui {
    class PrefsWindow;
}

class PrefsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrefsWindow(QWidget *parent = 0);
    ~PrefsWindow();
    void fe();


private:
    void resetSizePolicies(int index);
    Ui::PrefsWindow *ui;

private slots:
    void on_actionGeneral_triggered();
    void on_stackedWidget_currentChanged(int );
    void on_actionRecording_triggered();

public slots:
    void show(int startIndex);

signals:
    void fontChanged(QFont font);
    void bpmChanged(int bpm);
};

#endif // PREFSWINDOW_H
