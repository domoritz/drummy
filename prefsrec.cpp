#include "prefsrec.h"
#include "ui_prefsrec.h"
#include "noEditDelegate.h"

PrefsRec::PrefsRec(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PrefsRec)
{
    ui->setupUi(this);

    // set slider to saved value
    ui->bpmHorizontalSlider->setValue(settings.value("bpm",120).toInt());

    // avoid reading and writing of settings at the same time
    initalized = false;
    load_table_data_settings();
    settings.sync();
    initalized = true;
}

PrefsRec::~PrefsRec()
{
    delete ui;
}

/* someone changed the value of the slider */
void PrefsRec::on_bpmHorizontalSlider_valueChanged(int bpm)
{
    ui->bpmLineEdit->setText(QString::number(bpm));

    // save value to settings
    settings.setValue("bpm",bpm);
}

void PrefsRec::on_bpmLineEdit_editingFinished()
{
    bpm = ui->bpmLineEdit->text().toInt();
    ui->bpmHorizontalSlider->setValue(bpm);

    // save value to settings
    settings.setValue("bpm",bpm);
}

void PrefsRec::save_table_to_settings(){
    if (initalized) {

        int count = ui->treeWidget->topLevelItemCount();

        settings.beginWriteArray("mappings");
        for (int i = 0; i < count; i++) {
            settings.setArrayIndex(i);

            settings.setValue("name",ui->treeWidget->topLevelItem(i)->text(0));
            settings.setValue("key",ui->treeWidget->topLevelItem(i)->text(1));
            settings.setValue("char",ui->treeWidget->topLevelItem(i)->text(2));
            settings.setValue("active",ui->treeWidget->topLevelItem(i)->checkState(3));
        }
        settings.endArray();
    }
};

void PrefsRec::load_table_data_settings(){
    int count = settings.beginReadArray("mappings");

    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);

        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(0,settings.value("name","").toString());
        itm->setText(1,settings.value("key","").toString());
        itm->setText(2,settings.value("char","").toString());
        itm->setCheckState(3,settings.value("active",true).toBool()?Qt::Checked:Qt::Unchecked);

        itm->setFlags(itm->flags() | Qt::ItemIsEditable);

        // avoid editing of ckeckbox "text"
        ui->treeWidget->setItemDelegateForColumn(3, new NoEditDelegate(this));

        ui->treeWidget->insertTopLevelItem(i,itm);
    }
    settings.endArray();
}

void PrefsRec::on_addPushButton_clicked()
{
    int items = ui->treeWidget->topLevelItemCount();

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,"-");
    itm->setText(1,"-");
    itm->setText(2,"-");
    itm->setCheckState(3,Qt::Checked);

    itm->setFlags(itm->flags() | Qt::ItemIsEditable);

    // avoid editing of ckeckbox "text"
    ui->treeWidget->setItemDelegateForColumn(3, new NoEditDelegate(this));

    ui->treeWidget->insertTopLevelItem(items,itm);
}

void PrefsRec::on_removePushButton_clicked()
{
    QItemSelection selection(ui->treeWidget->selectionModel()->selection());

    QList<int> rows;
    foreach(const QModelIndex & index, selection.indexes()) {
        rows.append(index.row());
    }

    qSort(rows);

    int prev = -1;
    for(int i = rows.count() - 1; i >= 0; i -= 1) {
        int current = rows[i];
        if(current != prev) {
            ui->treeWidget->takeTopLevelItem(current);
            prev = current;
        }
    }
}

void PrefsRec::on_editPushButton_clicked()
{
    QList<QTreeWidgetItem *> selection = ui->treeWidget->selectedItems();
    ui->treeWidget->editItem(selection.first());
}

void PrefsRec::on_treeWidget_itemChanged(QTreeWidgetItem* item, int column)
{
    save_table_to_settings();
}
