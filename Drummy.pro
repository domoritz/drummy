#-------------------------------------------------
#
# Project created by QtCreator 2011-08-28T13:17:34
#
#-------------------------------------------------

QT       += core gui

TARGET = Drummy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    prefswindow.cpp \
    prefsapp.cpp \
    prefsrec.cpp \
    about.cpp \
    itemdialog.cpp \
    helpdialog.cpp \
    mappings.cpp \
    mappingitem.cpp

HEADERS  += mainwindow.h \
    prefswindow.h \
    prefsapp.h \
    prefsrec.h \
    about.h \
    itemdialog.h \
    noeditdelegate.h \
    helpdialog.h \
    mappings.h \
    mappingitem.h

FORMS    += mainwindow.ui \
    prefswindow.ui \
    prefsapp.ui \
    prefsrec.ui \
    about.ui \
    itemdialog.ui \
    helpdialog.ui

RESOURCES += \
    icons.qrc

OTHER_FILES +=

CONFIG += help \
          assistant

# in case you don't want a bundle
#CONFIG-=app_bundle
