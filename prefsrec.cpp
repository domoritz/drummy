#include "prefsrec.h"
#include "ui_prefsrec.h"
#include "noEditDelegate.h"
#include "itemdialog.h"
#include <QMessageBox>
#include <QStringList>
#include <QMenu>
#include "enums.h"

PrefsRec::PrefsRec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefsRec)
{
    ui->setupUi(this);

    // check if mappings are empty and disable widgets in case it is
    checkIfEmpty();

    // check selection and disable unneccessary widgets
    on_treeWidget_itemSelectionChanged();

    ui->treeWidget->addAction(ui->actionAdd);
    ui->treeWidget->addAction(ui->actionRemove);
    ui->treeWidget->addAction(ui->actionEdit);
    ui->treeWidget->addAction(ui->actionUp);
    ui->treeWidget->addAction(ui->actionDown);

    ui->moveToolButton->addAction(ui->actionUp);
    ui->moveToolButton->addAction(ui->actionDown);

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
        ui->defaultsPushButton->setEnabled(enabled);
        setSelectionButtonsEnabled(enabled);
    }
}

void PrefsRec::setMappingDefaults()
{
    // Overwrite with defaults was clicked
    ui->treeWidget->clear();

    QStringList Hh;
    Hh << "High-hat" << "Hh" << "h" << "x";
    QTreeWidgetItem *Hhitm = new QTreeWidgetItem(QStringList(Hh));
    Hhitm->setCheckState(MACTIVE,Qt::Checked);
    Hhitm->setCheckState(MENABLED,Qt::Checked);
    ui->treeWidget->insertTopLevelItem(0,Hhitm);

    QStringList TT;
    TT << "Low tom" << "T" << "t" << "t";
    QTreeWidgetItem *TTitm = new QTreeWidgetItem(QStringList(TT));
    TTitm->setCheckState(MACTIVE,Qt::Checked);
    TTitm->setCheckState(MENABLED,Qt::Checked);
    ui->treeWidget->insertTopLevelItem(1,TTitm);

    QStringList R;
    R << "Crash" << "Cc" << "c" << "X";
    QTreeWidgetItem *Ritm = new QTreeWidgetItem(QStringList(R));
    Ritm->setCheckState(MACTIVE,Qt::Checked);
    Ritm->setCheckState(MENABLED,Qt::Checked);
    ui->treeWidget->insertTopLevelItem(2,Ritm);

    this->saveTableToSettings();
}

void PrefsRec::saveTableToSettings(){
    if (initalized) {

        int count = ui->treeWidget->topLevelItemCount();

        settings.beginWriteArray("mappings");
        for (int i = 0; i < count; i++) {
            settings.setArrayIndex(i);

            settings.setValue("name",ui->treeWidget->topLevelItem(i)->text(MNAME));
            settings.setValue("shortname",ui->treeWidget->topLevelItem(i)->text(MSNAME));
            settings.setValue("key",ui->treeWidget->topLevelItem(i)->text(MKEY));
            settings.setValue("char",ui->treeWidget->topLevelItem(i)->text(MCHAR));
            settings.setValue("active",ui->treeWidget->topLevelItem(i)->checkState(MACTIVE));
            settings.setValue("enabled",ui->treeWidget->topLevelItem(i)->checkState(MENABLED));
        }
        settings.endArray();
    }

    // adjust column widths
    for (int i=0;i<6;i++)
        ui->treeWidget->resizeColumnToContents(i);

    emit settingsChanged();
};

void PrefsRec::load_table_data_settings(){
    int count = settings.beginReadArray("mappings");

    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);

        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(MNAME,settings.value("name","").toString());
        itm->setText(MSNAME,settings.value("shortname","").toString());
        itm->setText(MKEY,settings.value("key","").toString());
        itm->setText(MCHAR,settings.value("char","").toString());
        itm->setCheckState(MACTIVE,settings.value("active",true).toBool()?Qt::Checked:Qt::Unchecked);
        itm->setCheckState(MENABLED,settings.value("enabled",true).toBool()?Qt::Checked:Qt::Unchecked);

        itm->setFlags(itm->flags() | Qt::ItemIsEditable);

        // avoid editing of ckeckbox "active" and "enabled"
        ui->treeWidget->setItemDelegateForColumn(MACTIVE, new NoEditDelegate(this));
        ui->treeWidget->setItemDelegateForColumn(MENABLED, new NoEditDelegate(this));

        ui->treeWidget->insertTopLevelItem(i,itm);
    }
    settings.endArray();

    // adjust column widths
    for (int i=0;i<MENABLED;i++)
        ui->treeWidget->resizeColumnToContents(i);
}

void PrefsRec::on_treeWidget_itemChanged(QTreeWidgetItem* item, int column)
{
    saveTableToSettings();
    checkIfEmpty();
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
    if (ui->editToolButton->isEnabled()) {
        editItem(item, column);
    }
}

void PrefsRec::on_defaultsPushButton_clicked()
{
    if (ui->treeWidget->topLevelItemCount() == 0){
        setMappingDefaults();
        return;
    }

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

void PrefsRec::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}

void PrefsRec::on_actionAdd_triggered()
{
    int items = ui->treeWidget->topLevelItemCount();

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(MNAME,"");
    itm->setText(MSNAME,"");
    itm->setText(MKEY,"");
    itm->setText(MCHAR,"");
    itm->setCheckState(MACTIVE,Qt::Checked);
    itm->setCheckState(MENABLED,Qt::Checked);

    itm->setFlags(itm->flags() | Qt::ItemIsEditable);

    // avoid editing of ckeckbox "text"
    ui->treeWidget->setItemDelegateForColumn(MACTIVE, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(MENABLED, new NoEditDelegate(this));

    ui->treeWidget->insertTopLevelItem(items,itm);

    editItem(itm,-1);
}

void PrefsRec::on_actionEdit_triggered()
{
    QList<QTreeWidgetItem *> selection = ui->treeWidget->selectedItems();

    if (selection.isEmpty())
        return;

    editItem(selection.first(),-1);
}

void PrefsRec::on_actionRemove_triggered()
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

    checkIfEmpty();
    saveTableToSettings();
}

void PrefsRec::on_actionUp_triggered()
{
    QList<QTreeWidgetItem *> selection = ui->treeWidget->selectedItems();

    if (selection.isEmpty())
        return;

    // If selection has the first line the items are on the top -> return
    int row = ui->treeWidget->indexOfTopLevelItem(selection.first());
    if ( row == 0 )
        return;

    QTreeWidgetItem *itemAbove;
    QListIterator<QTreeWidgetItem *> it(selection);

    while ( it.hasNext() )
    {
        row = ui->treeWidget->indexOfTopLevelItem(it.next());
        itemAbove = ui->treeWidget->takeTopLevelItem(row - 1);
        ui->treeWidget->insertTopLevelItem(row, itemAbove);
    }
}

void PrefsRec::on_actionDown_triggered()
{
    QList<QTreeWidgetItem *> selection = ui->treeWidget->selectedItems();

    if (selection.isEmpty())
        return;

    // If selection has the first line the items are on the top -> return
    int row = ui->treeWidget->indexOfTopLevelItem(selection.last());
    if (row == ui->treeWidget->topLevelItemCount() - 1)
        return;

    QTreeWidgetItem *itemBelow;

    row = ui->treeWidget->indexOfTopLevelItem(selection.last());
    itemBelow = ui->treeWidget->takeTopLevelItem( row + 1);
    ui->treeWidget->insertTopLevelItem(row + 1 - selection.length() , itemBelow);

    ui->treeWidget->scrollToItem(selection.last());
}

void PrefsRec::on_moveToolButton_clicked()
{
    ui->moveToolButton->showMenu();
}

// disbale some widgets if treewidget is empty
void PrefsRec::checkIfEmpty()
{
    setSelectionButtonsEnabled(ui->treeWidget->topLevelItemCount() != 0);

    // check selection
    on_treeWidget_itemSelectionChanged();
}

void PrefsRec::on_treeWidget_itemSelectionChanged()
{
    setSelectionButtonsEnabled(ui->treeWidget->selectedItems().count() != 0);
}

void PrefsRec::setSelectionButtonsEnabled(bool enabled)
{
    ui->moveToolButton->setEnabled(enabled);
    ui->removeToolButton->setEnabled(enabled);
    ui->editToolButton->setEnabled(enabled);
    ui->actionUp->setEnabled(enabled);
    ui->actionDown->setEnabled(enabled);
    ui->actionRemove->setEnabled(enabled);
    ui->actionEdit->setEnabled(enabled);
}
