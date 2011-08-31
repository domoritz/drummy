#include "itemdialog.h"
#include "ui_itemdialog.h"
#include <QTableWidget>
#include <QDebug>

ItemDialog::ItemDialog(PrefsRec *parent) :
    QDialog(parent),
    ui(new Ui::ItemDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted),parent,SLOT(save_table_to_settings));
}

ItemDialog::~ItemDialog()
{
    delete ui;
}

void ItemDialog::setItem(QTreeWidgetItem *item, int column)
{
    this->item = item;
    ui->nameLineEdit->setText(item->text(0));
    ui->keyLineEdit->setText(item->text(1));
    ui->charLineEdit->setText(item->text(2));
    ui->activeCheckBox->setCheckState(item->checkState(3));

    switch(column) {
    case 0:
        ui->nameLineEdit->setFocus();
        break;
    case 1:
        ui->keyLineEdit->setFocus();
        break;
    case 2:
        ui->charLineEdit->setFocus();
        break;
    case 3:
        ui->activeCheckBox->setFocus();
        break;
    default:
        break;
    }

};


// add/ save
void ItemDialog::on_buttonBox_accepted()
{
    item->setText(0,ui->nameLineEdit->text());
    item->setText(1,ui->keyLineEdit->text());
    item->setText(2,ui->charLineEdit->text());
    item->setCheckState(3,ui->activeCheckBox->checkState());

    this->close();
}


// cancel
void ItemDialog::on_buttonBox_rejected()
{
    this->close();
}
