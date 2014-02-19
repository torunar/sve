#ifndef PLUGINLISTWINDOW_H
#define PLUGINLISTWINDOW_H

#include <QDialog>
#include <QStatusBar>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QTreeWidgetItem>
#include "plugin.h"

#include <QDebug>

namespace Ui {
class PluginListWindow;
}

enum class LCol{
    isEnabled = 0,
    pluginName = 1,
    isOnPanel = 2,
    author = 3,
    description = 4

};

class PluginListWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit PluginListWindow(QWidget *parent = 0);
    ~PluginListWindow();
    
private:
    Ui::PluginListWindow *ui;
    QSettings *settings;
    QDir pluginsDir;
    QList<QTreeWidgetItem*> loadPluginsList();

public slots:
    void refreshList();
    void save();
};

#endif // PLUGINLISTWINDOW_H
