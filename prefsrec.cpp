#include "prefsrec.h"
#include "ui_prefsrec.h"
#include "noEditDelegate.h"
#include "itemdialog.h"
#include <QMessageBox>
#include <QStringList>

PrefsRec::PrefsRec(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PrefsRec)
{
    ui->setupUi(this);

    reload();
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
    emit bpmChanged(bpm);
}

void PrefsRec::on_bpmLineEdit_editingFinished()
{
    bpm = ui->bpmLineEdit->text().toInt();
    ui->bpmHorizontalSlider->setValue(bpm);

    // save value to settings
    settings.setValue("bpm",bpm);
    emit bpmChanged(bpm);
}

void PrefsRec::reload()
{
    // set slider to saved value
    ui->bpmHorizontalSlider->setValue(settings.value("bpm",120).toInt());

    // avoid reading and writing of settings at the same time
    initalized = false;
    ui->treeWidget->clear();
    load_table_data_settings();
    settings.sync();
    initalized = true;
}

// disables/enables everything in the mapping group
void PrefsRec::setFullyEnabled(bool enabled, bool excludeButtons)
{
    ui->treeWidget->setEnabled(enabled);
    ui->bpmLineEdit->setEnabled(enabled);

    if (!excludeButtons) {
        ui->editPushButton->setEnabled(enabled);
        ui->addPushButton->setEnabled(enabled);
        ui->defaultsPushButton->setEnabled(enabled);
        ui->removePushButton->setEnabled(enabled);
    }
}

void PrefsRec::setMappingDefaults()
{
    // Overwrite with defaults was clicked
    ui->treeWidget->clear();

    QStringList Hh;
    Hh << "Hh" << "h" << "x";
    QTreeWidgetItem *Hhitm = new QTreeWidgetItem(QStringList(Hh));
    Hhitm->setCheckState(3,Qt::Checked);
    ui->treeWidget->insertTopLevelItem(0,Hhitm);

    QStringList TT;
    TT << "TT" << "t" << "x";
    QTreeWidgetItem *TTitm = new QTreeWidgetItem(QStringList(TT));
    TTitm->setCheckState(3,Qt::Checked);
    ui->treeWidget->insertTopLevelItem(1,TTitm);

    QStringList R;
    R << "R" << "r" << "o";
    QTreeWidgetItem *Ritm = new QTreeWidgetItem(QStringList(R));
    Ritm->setCheckState(3,Qt::Checked);
    ui->treeWidget->insertTopLevelItem(2,Ritm);

    this->saveTableToSettings();
}

void PrefsRec::saveTableToSettings(){
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

    emit settingsChanged();
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
    itm->setText(0,"");
    itm->setText(1,"");
    itm->setText(2,"");
    itm->setCheckState(3,Qt::Checked);

    itm->setFlags(itm->flags() | Qt::ItemIsEditable);

    // avoid editing of ckeckbox "text"
    ui->treeWidget->setItemDelegateForColumn(3, new NoEditDelegate(this));

    ui->treeWidget->insertTopLevelItem(items,itm);

    editItem(itm,-1);
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

    saveTableToSettings();
}

void PrefsRec::on_editPushButton_clicked()
{
    QList<QTreeWidgetItem *> selection = ui->treeWidget->selectedItems();
    //ui->treeWidget->editItem(selection.first());
    if (!selection.empty()) {
        editItem(selection.first(),-1);
    }
}

void PrefsRec::on_treeWidget_itemChanged(QTreeWidgetItem* item, int column)
{
    saveTableToSettings();
}

void PrefsRec::editItem(QTreeWidgetItem* item, int column)
{

    ItemDialog dialog(this);
    dialog.setModal(true);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setItem(item, column);
    dialog.exec();
}

void PrefsRec::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    // before editing an item check if it may not be adequate
    if (ui->editPushButton->isEnabled()) {
        editItem(item, column);
}
}

void PrefsRec::on_defaultsPushButton_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setModal(true);
    msgBox.setWindowModality(Qt::WindowModal);
    msgBox.setText("Replace current settings with defaults?");
    msgBox.setInformativeText("Do you want to replace the current mappings with some example settings? Your own mappings will be lost.");
    msgBox.setStandardButtons(QMessageBox::Yes  | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes) {
        setMappingDefaults();
    } else {
        // Don't overwrite was clicked

    }
}
