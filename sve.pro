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
    elementnode.cpp \
    nodepropertieswindow.cpp \
    plugin.cpp \
    pluginlistwindow.cpp \
    addnodedialog.cpp \
    unode.cpp \
    labelnode.cpp \
    linknode.cpp

HEADERS  += mainwindow.h \
    document.h \
    docwindow.h \
    nodepropertieswindow.h \
    plugin.h \
    pluginlistwindow.h \
    addnodedialog.h \
    unode.h \
    linknode.h \
    elementnode.h \
    labelnode.h

FORMS    += mainwindow.ui \
    nodepropertieswindow.ui \
    pluginlistwindow.ui \
    addnodedialog.ui

RESOURCES += \
    ui.qrc

QMAKE_CXXFLAGS += -std=c++11
