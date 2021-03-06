#-------------------------------------------------
#
# Project created by QtCreator 2013-04-28T23:14:28
#
#-------------------------------------------------

QT       += webkitwidgets core gui xml

TARGET = sve
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    document.cpp \
    docwindow.cpp \
    elementnode.cpp \
    plugin.cpp \
    pluginlistwindow.cpp \
    addnodedialog.cpp \
    unode.cpp \
    labelnode.cpp \
    linknode.cpp \
    connectiondialog.cpp \
    documentoptionsdialog.cpp \
    preferencesdialog.cpp \
    sourceviewdialog.cpp \
    nodepropertiesdialog.cpp

HEADERS  += mainwindow.h \
    document.h \
    docwindow.h \
    plugin.h \
    pluginlistwindow.h \
    addnodedialog.h \
    unode.h \
    linknode.h \
    elementnode.h \
    labelnode.h \
    connectiondialog.h \
    documentoptionsdialog.h \
    preferencesdialog.h \
    sourceviewdialog.h \
    nodepropertiesdialog.h

FORMS    += mainwindow.ui \
    pluginlistwindow.ui \
    addnodedialog.ui \
    connectiondialog.ui \
    documentoptionsdialog.ui \
    preferencesdialog.ui \
    sourceviewdialog.ui \
    nodepropertiesdialog.ui

RESOURCES += \
    ui.qrc

QMAKE_CXXFLAGS += -std=c++11

TRANSLATIONS = sve_ru.ts
