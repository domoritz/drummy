#ifndef PREFSREC_H
#define PREFSREC_H

#include <QtDebug>
#include <QWidget>
#include <QSettings>
#include <QTreeWidgetItem>

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

protected:
    int bpm;
    QSettings settings;
    void load_table_data_settings();
    bool initalized;

private slots:
    void on_defaultsPushButton_clicked();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_itemChanged(QTreeWidgetItem* item, int column);
    void on_bpmLineEdit_editingFinished();
    void on_bpmHorizontalSlider_valueChanged(int value);
    void editItem(QTreeWidgetItem* item, int row);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_actionAdd_triggered();
    void on_actionEdit_triggered();
    void on_actionRemove_triggered();
    void on_actionUp_triggered();
    void on_actionDown_triggered();
    void on_moveToolButton_clicked();
    void checkIfEmpty();
    void on_treeWidget_itemSelectionChanged();
    void setSelectionButtonsEnabled(bool enabled);

    void on_metronomPushButton_clicked();

public slots:
    void reload();
    void reloadBmp(int bpm=0);
    void saveTableToSettings();
    void setFullyEnabled(bool enabled, bool excludeButtons=false);
    void setMappingDefaults();

signals:
    void bpmChanged(int bpm);
    void settingsChanged();
};

#endif // PREFSREC_H
