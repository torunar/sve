#-------------------------------------------------
#
# Project created by QtCreator 2013-04-28T23:14:28
#
#-------------------------------------------------

QT       += core gui webkitwidgets xml

TARGET = sve
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    document.cpp \
    docwindow.cpp \
    editablelabel.cpp \
    elementnode.cpp \
    nodepropertieswindow.cpp \
    plugin.cpp \
    pluginlistwindow.cpp

HEADERS  += mainwindow.h \
    document.h \
    docwindow.h \
    editablelabel.h \
    elementnode.h \
    nodepropertieswindow.h \
    plugin.h \
    pluginlistwindow.h

FORMS    += mainwindow.ui \
    nodepropertieswindow.ui \
    pluginlistwindow.ui

RESOURCES += \
    ui.qrc
