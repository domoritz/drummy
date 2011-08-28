#ifndef PREFSWINDOW_H
#define PREFSWINDOW_H

#include <QMainWindow>

namespace Ui {
    class PrefsWindow;
}

class PrefsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrefsWindow(QWidget *parent = 0);
    ~PrefsWindow();

private:
    Ui::PrefsWindow *ui;

private slots:
    void on_actionRecording_triggered();
    void on_actionApplication_triggered();
};

#endif // PREFSWINDOW_H
