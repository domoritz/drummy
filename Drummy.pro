######################################################################
# Automatically generated by qmake (2.01a) Tue Aug 30 11:31:20 2011
#
# update lang:
# lupdate -no-obsolete Drummy.pro
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

win32 {
    win32:RC_FILE = Drummy.rc
    target.path = /bin
} else:macx {
    LIBS += -framework CoreFoundation
    ICON = images/Drummy.icns
    QMAKE_INFO_PLIST = drummy.Info.plist
} else {
    LIBS *= -lExtensionSystem -lAggregation

    target.path  = /bin
    INSTALLS    += target
}

# Input
HEADERS += about.h \
           helpdialog.h \
           itemdialog.h \
           mainwindow.h \
           mappingitem.h \
           mappings.h \
           noeditdelegate.h \
           prefsapp.h \
           prefsrec.h \
           prefswindow.h \
    painter.h
FORMS += about.ui \
         helpdialog.ui \
         itemdialog.ui \
         mainwindow.ui \
         prefsapp.ui \
         prefsrec.ui \
         prefswindow.ui
SOURCES += about.cpp \
           helpdialog.cpp \
           itemdialog.cpp \
           main.cpp \
           mainwindow.cpp \
           mappingitem.cpp \
           mappings.cpp \
           prefsapp.cpp \
           prefsrec.cpp \
           prefswindow.cpp \
           painter.cpp
TRANSLATIONS += lang/drummy_en.ts \
                lang/drummy_de.ts
CODECFORTC = UTF-8
RESOURCES += icons.qrc \
             languages.qrc

OTHER_FILES += \
    Drummy.rc
