#include "songinfo.h"
#include "ui_songinfo.h"

SongInfo::SongInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongInfo)
{
    ui->setupUi(this);
}

SongInfo::~SongInfo()
{
    delete ui;
}

QString SongInfo::stringifyInfos()
{
    QString out;

    int size = -15;

    if (!ui->titleLineEdit->text().isEmpty()) {
        out.append(QString("%1: %2\n").arg(tr("Song"),size).arg(ui->titleLineEdit->text()));
    }

    if (!ui->artistLineEdit->text().isEmpty()) {
        out.append(QString("%1: %2\n").arg(tr("Artist"),size).arg(ui->artistLineEdit->text()));
    }

    if (!ui->albumLineEdit->text().isEmpty()) {
        out.append(QString("%1: %2\n").arg(tr("Album"),size).arg(ui->albumLineEdit->text()));
    }

    if (ui->timeEdit->time().toString("H:m:s") != "0:0:0") {
        out.append(QString("%1: %2\n").arg(tr("Length"),size).arg(ui->timeEdit->time().toString("HH:mm:ss")));
    }

    if (!out.isEmpty()) {
        out.append("\n");
    }

    out.append(QString("%1: %2\n").arg(tr("Instrument"),size).arg(ui->instrumentComboBox->itemText(ui->instrumentComboBox->currentIndex())));

    if (!ui->authorLineEdit->text().isEmpty()) {
        out.append(QString("%1: %2\n").arg(tr("Author"),size).arg(ui->authorLineEdit->text()));
    }

    // will be replaced with mappings
    out.append("\n##mappings##\n\n");

    if (!ui->textEdit->toPlainText().isEmpty()) {
        out.append(QString("%1:\n%2\n").arg(tr("Additional Information")).arg(ui->textEdit->toPlainText()));
    }

    return out;
}

void SongInfo::clearUi()
{
    ui->titleLineEdit->clear();
    ui->albumLineEdit->clear();
    ui->artistLineEdit->clear();
    ui->authorLineEdit->clear();
    ui->timeEdit->setTime(QTime(0,0,0,0));
    ui->textEdit->clear();
}
