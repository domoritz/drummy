#include "about.h"
#include "ui_about.h"
#include "version.c"
#include <QString>

About::About(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::About)
{
    ui->setupUi(this);

    QString buildinfo;
    buildinfo += "Built on ";
    buildinfo += build_date;
    buildinfo += "<br>GIT sha: ";
    buildinfo += build_git_sha;

    ui->buildinfoLabel->setText(buildinfo);

    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
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

