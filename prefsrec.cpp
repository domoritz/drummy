#include "prefsrec.h"
#include "ui_prefsrec.h"

PrefsRec::PrefsRec(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PrefsRec)
{
    ui->setupUi(this);

    // set slider to saved value
    ui->bpmHorizontalSlider->setValue(settings.value("bpm",120).toInt());

    load_table_data_settings();
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
    int count = ui->tableWidget->rowCount();
    settings.setValue("mapping.count",count);

    settings.beginWriteArray("mappings");
    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);

        // null pointer could occur if you just use tableWidget->item(?,?)
        settings.setValue("name",ui->tableWidget->item(i,0) ? ui->tableWidget->item(i,0)->text() : "");
        settings.setValue("key",ui->tableWidget->item(i,1) ? ui->tableWidget->item(i,1)->text() : "");
        settings.setValue("char",ui->tableWidget->item(i,2) ? ui->tableWidget->item(i,2)->text() : "");
    }
    settings.endArray();
};

void PrefsRec::load_table_data_settings(){

}

void PrefsRec::on_addPushButton_clicked()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    //save_table_to_settings();
}

void PrefsRec::on_removePushButton_clicked()
{
    QItemSelection selection(ui->tableWidget->selectionModel()->selection());

    QList<int> rows;
    foreach(const QModelIndex & index, selection.indexes()) {
        rows.append(index.row());
    }

    qSort(rows);

    int prev = -1;
    for(int i = rows.count() - 1; i >= 0; i -= 1) {
        int current = rows[i];
        if(current != prev) {
            ui->tableWidget->removeRow(current);
            prev = current;
        }
    }
}

void PrefsRec::on_tableWidget_cellChanged(int row, int column)
{
    save_table_to_settings();
}
