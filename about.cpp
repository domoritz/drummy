#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::on_buttonBox_accepted()
{
    this->close();
}

void About::on_aboutQTbutton_clicked()
{
    QApplication::aboutQt();
}