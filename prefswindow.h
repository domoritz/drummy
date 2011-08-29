#ifndef PREFSWINDOW_H
#define PREFSWINDOW_H

#include <QMainWindow>
#include <QtDebug>

namespace Ui {
    class PrefsWindow;
}

class PrefsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrefsWindow(QWidget *parent = 0,int startIndex = 0);
    ~PrefsWindow();

private:
    void resetSizePolicies(int index);
    Ui::PrefsWindow *ui;

private slots:
    void on_stackedWidget_currentChanged(int );
    void on_actionRecording_triggered();
    void on_actionApplication_triggered();
};

#endif // PREFSWINDOW_H
