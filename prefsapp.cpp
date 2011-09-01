#include "prefsapp.h"
#include "ui_prefsapp.h"
#include <QDebug>

PrefsApp::PrefsApp(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PrefsApp)
{
    ui->setupUi(this);

    #ifdef Q_WS_MACX
    QString family = "Monaco";
    int size = 13;
    #else
    // non Mac code here
    QString family = "Courier"
    int size = 13;
    #endif

    // load settings
    outputfont.setFamily(settings.value("font/family",family).toString());
    outputfont.setPointSize(settings.value("font/size",size).toInt());

    ui->progressCheckBox->setChecked(settings.value("progress",true).toBool());

    saveFont(outputfont);
}

void PrefsApp::fe(){emit fontChanged(outputfont);};

PrefsApp::~PrefsApp()
{
    delete ui;
}

void PrefsApp::on_pushButton_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this->outputfont, this);

    if (ok) {
        // the user clicked OK and font is set to the font the user selected
        outputfont = font;
        saveFont(outputfont);
    } else {
        // the user canceled the dialog; font is set to the initial value
    }
}

void PrefsApp::saveFont(QFont font)
{
    QFont copy = font;
    copy.setPointSize(12);
    ui->fontLineEdit->setFont(copy);
    ui->fontLineEdit->setText(font.family() + " " + QString::number(font.pointSize()));

    settings.setValue("font/family",font.family());
    settings.setValue("font/size",font.pointSize());

    emit fontChanged(outputfont);
}

void PrefsApp::on_progressCheckBox_toggled(bool checked)
{
    settings.setValue("progress",checked);
}
