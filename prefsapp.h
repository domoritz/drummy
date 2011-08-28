#ifndef PREFSAPP_H
#define PREFSAPP_H

#include <QWidget>

namespace Ui {
    class PrefsApp;
}

class PrefsApp : public QWidget
{
    Q_OBJECT

public:
    explicit PrefsApp(QWidget *parent = 0);
    ~PrefsApp();

private:
    Ui::PrefsApp *ui;
};

#endif // PREFSAPP_H
