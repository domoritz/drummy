#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set up information about app
    QCoreApplication::setOrganizationName("Dominik");
    QCoreApplication::setOrganizationDomain("dominik.de");
    QCoreApplication::setApplicationName("Drummy");

    QCoreApplication::setApplicationVersion("0.1");

    MainWindow w;
    w.show();

    return a.exec();
}
