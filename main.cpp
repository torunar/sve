#include <QtWidgets/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QSettings>

#include "pluginlistwindow.h"

int main(int argc, char *argv[])
{
    QSettings settings("torunar", "sve");
    // Blank document settings
    settings.beginGroup("DefaultDoc");
    if (settings.value("ListSize",  "") == "") {
        settings.setValue("ListSize",  QSize(980, 560));
    }
    if (settings.value("NodeSize",  "") == "") {
        settings.setValue("NodeSize",  QSize(80, 60));
    }
    settings.endGroup();
    // Plugins
    settings.beginGroup("Plugins");
    if (settings.value("PluginDir", "") == "") {
        settings.setValue("PluginDir", "\"./plugins/\"");
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

    PluginListWindow pw;
    pw.show();
    return a.exec();
}
