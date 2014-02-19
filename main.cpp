#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QSettings>

int main(int argc, char *argv[])
{
    QSettings settings("torunar", "sve");
    // Blank document settings
    settings.beginGroup("default_doc");
    if (settings.value("blank_size",  "") == "") {
        settings.setValue("blank_size",  QSize(980, 560));
    }
    if (settings.value("node_size",  "") == "") {
        settings.setValue("node_size",  QSize(80, 60));
    }
    settings.endGroup();
    // Plugins
    settings.beginGroup("plugins");
    if (settings.value("plugin_dir", "") == "") {
        settings.setValue("plugin_dir", "./plugins/");
    }
    settings.endGroup();

    // no longer required with migration to qt5
    /*
    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);
    */

    QApplication a(argc, argv);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
