#ifndef PREFSAPP_H
#define PREFSAPP_H

#include <QWidget>
#include <QSettings>
#include <QFontDialog>

namespace Ui {
    class PrefsApp;
}

class PrefsApp : public QWidget
{
    Q_OBJECT

public:
    explicit PrefsApp(QWidget *parent = 0);
    ~PrefsApp();
    void fe();

protected:
    QFont outputfont;
    QSettings settings;

private:
    Ui::PrefsApp *ui;

private slots:
    void on_barsperlineSpinBox_valueChanged(int );
    void on_progressCheckBox_toggled(bool checked);
    void on_pushButton_clicked();

    void on_resetPushButton_clicked();

public slots:
    void saveFont(QFont font);

signals:
    void fontChanged(QFont font);

};

#endif // PREFSAPP_H
