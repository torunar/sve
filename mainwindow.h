#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWebKitWidgets/QWebView>
#include <QPainterPath>

#include "docwindow.h"
#include "pluginlistwindow.h"
#include "preferencesdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    Document *activeDocument;
    DocWindow *activeWindow;

    ~MainWindow();
    QSettings *settings;

    void initPluginsToolbar();
    
private:
    QList<Plugin *> plugins;
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
    void loadPlugins();
    void disconnectSlots();
    void connectSlots();

public slots:
    void createDocument();
    void setActiveDocument();
    void quit();
    void load();
    void showPluginListWindow();
    void findPlugin();
    void showPreferencesDialog();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
