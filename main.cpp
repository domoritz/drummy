#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>
#include <QTranslator>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set up information about app
    QCoreApplication::setOrganizationName("Dominik");
    QCoreApplication::setOrganizationDomain("dominik.de");
    QCoreApplication::setApplicationName("Drummy");

    QCoreApplication::setApplicationVersion("1.0 Beta 1");

    QApplication::setWindowIcon(QIcon(":/images/record_512.png"));

    // set encoding to utf-8
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

#ifdef QT_DEBUG
    qDebug() << "Application compiled in debug mode";
    //do Debug stuff here
#endif

#ifdef QT_NO_DEBUG
    qDebug() << "Application compiled in release mode";
    //do release stuff here
#endif

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
