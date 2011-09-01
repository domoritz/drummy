#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set up information about app
    QCoreApplication::setOrganizationName("Dominik");
    QCoreApplication::setOrganizationDomain("dominik.de");
    QCoreApplication::setApplicationName("Drummy");

    QCoreApplication::setApplicationVersion("0.1");

    // set encoding to utf-8
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    // Translator
    QTranslator qtTranslator;
    qDebug() << "qt language" << qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    qDebug() << "drummy language" << appTranslator.load("drummy_" + QLocale::system().name(),":/lang");
    a.installTranslator(&appTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
