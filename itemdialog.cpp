#include "itemdialog.h"
#include "ui_itemdialog.h"
#include <QTableWidget>

ItemDialog::ItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemDialog)
{
    ui->setupUi(this);
}

ItemDialog::~ItemDialog()
{
    delete ui;
}


// add/ save
void ItemDialog::on_buttonBox_accepted()
{

}


// cancel
void ItemDialog::on_buttonBox_rejected()
{

}
