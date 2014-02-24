#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QSettings>

int main(int argc, char *argv[])
{
    QSettings settings("torunar", "sve");
    // Blank document settings
    if (settings.value("default_doc/blank_size",  "") == "") {
        settings.setValue("default_doc/blank_size",  QSize(980, 560));
    }
    if (settings.value("default_doc/node_size",  "") == "") {
        settings.setValue("default_doc/node_size",  QSize(80, 60));
    }
    // Plugins
    if (settings.value("plugins/plugin_dir", "") == "") {
        settings.setValue("plugins/plugin_dir", "./plugins/");
    }
    if (settings.value("plugins/icon_size", "") == "") {
        settings.setValue("plugins/icon_size",  QSize(32, 32));
    }

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
