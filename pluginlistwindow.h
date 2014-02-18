#ifndef PLUGINLISTWINDOW_H
#define PLUGINLISTWINDOW_H

#include <QDialog>
#include <QStatusBar>
#include <QFile>
#include <QDir>
#include <QSettings>
#include "plugin.h"

#include <QDebug>

namespace Ui {
class PluginListWindow;
}

class PluginListWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit PluginListWindow(QWidget *parent = 0);
    ~PluginListWindow();
    
private:
    Ui::PluginListWindow *ui;
    QSettings *settings;

public slots:
    void refreshList();
    void save() {}
};

#endif // PLUGINLISTWINDOW_H
