#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QSettings>

int main(int argc, char *argv[])
{
    QSettings settings("torunar", "sve");
    settings.beginGroup("DefaultDoc");
    if (settings.value("Size",  "") == "") settings.setValue("Size",  QSize(980, 560));
    settings.endGroup();

    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    
    return a.exec();
}
