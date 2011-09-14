#include "itemdialog.h"
#include "ui_itemdialog.h"
#include <QTableWidget>
#include <QDebug>
#include "enums.h"

ItemDialog::ItemDialog(PrefsRec *parent) :
    QDialog(parent),
    ui(new Ui::ItemDialog)
{
    ui->setupUi(this);
}

ItemDialog::~ItemDialog()
{
    delete ui;
}

void ItemDialog::setItem(QTreeWidgetItem *item, int column)
{
    this->item = item;
    ui->nameLineEdit->setText(item->text(MNAME));
    ui->shortNameLineEdit->setText(item->text(MSNAME));
    ui->keyLineEdit->setText(item->text(MKEY));
    ui->charLineEdit->setText(item->text(MCHAR));
    ui->activeCheckBox->setCheckState(item->checkState(MACTIVE));
    ui->enabledCheckBox->setCheckState(item->checkState(MENABLED));

    switch(column) {
    case MNAME:
        ui->nameLineEdit->setFocus();
        break;
    case MSNAME:
        ui->shortNameLineEdit->setFocus();
        break;
    case MKEY:
        ui->keyLineEdit->setFocus();
        break;
    case MCHAR:
        ui->charLineEdit->setFocus();
        break;
    case MACTIVE:
        ui->activeCheckBox->setFocus();
        break;
    case MENABLED:
        ui->enabledCheckBox->setFocus();
        break;
    default:
        break;
    }

};


// add/ save
void ItemDialog::on_buttonBox_accepted()
{
    item->setText(MNAME,ui->nameLineEdit->text());
    item->setText(MSNAME,ui->shortNameLineEdit->text());
    item->setText(MKEY,ui->keyLineEdit->text());
    item->setText(MCHAR,ui->charLineEdit->text());
    item->setCheckState(MACTIVE,ui->activeCheckBox->checkState());
    item->setCheckState(MENABLED,ui->enabledCheckBox->checkState());

    this->close();
}


// cancel
void ItemDialog::on_buttonBox_rejected()
{
    this->close();
}
