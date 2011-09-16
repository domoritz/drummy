#ifndef METRONOMDIALOG_H
#define METRONOMDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>
#include <QAbstractButton>

namespace Ui {
    class MetronomDialog;
}

class MetronomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetronomDialog(QWidget *parent = 0);
    ~MetronomDialog();
    int getBpm();

private slots:
    void keyPressEvent ( QKeyEvent * event );
    void refreshTime();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::MetronomDialog *ui;
    QTime time;
    int lastTimeInMsec;
    QTimer timer;
    int bpm;
};

#endif // METRONOMDIALOG_H
