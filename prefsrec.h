#ifndef PREFSREC_H
#define PREFSREC_H

#include <QtDebug>
#include <QWidget>
#include <QSettings>

namespace Ui {
    class PrefsRec;
}

class PrefsRec : public QWidget
{
    Q_OBJECT

public:
    explicit PrefsRec(QWidget *parent = 0);
    ~PrefsRec();

private:
    Ui::PrefsRec *ui;
    int bpm;
    QSettings settings;
    void save_table_to_settings();
    void load_table_data_settings();

private slots:
    void on_tableWidget_cellChanged(int row, int column);
    void on_removePushButton_clicked();
    void on_addPushButton_clicked();
    void on_bpmLineEdit_editingFinished();
    void on_bpmHorizontalSlider_valueChanged(int value);
};

#endif // PREFSREC_H
