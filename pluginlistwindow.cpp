#include "pluginlistwindow.h"
#include "ui_pluginlistwindow.h"

PluginListWindow::PluginListWindow(QWidget *parent) : QDialog(parent), ui(new Ui::PluginListWindow) {
    ui->setupUi(this);
    this->settings = new QSettings("mike-schekotov", "sve");

    // plugins root
    QString pluginsPath = this->settings->value("plugins/plugin_dir", "./plugins/").toString();
    this->pluginsDir    = QDir(pluginsPath);

    // load plugins list on start
    QList<QTreeWidgetItem*> plugins = this->loadPluginsList();
    this->ui->pluginList->insertTopLevelItems(0, plugins);

    connect(this->ui->refreshButton, SIGNAL(clicked()), this, SLOT(refreshList()));
    connect(this->ui->cancelButton,  SIGNAL(clicked()), this, SLOT(reject()));
    connect(this->ui->okButton,      SIGNAL(clicked()), this, SLOT(save()));
}

PluginListWindow::~PluginListWindow() {
    delete ui;
}

// load list of plugins from disk
QList<QTreeWidgetItem *> PluginListWindow::loadPluginsList() {
    QList<QTreeWidgetItem*> items;

    // possible plugins
    QStringList plugins        = this->pluginsDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    QStringList enabledPlugins = this->settings->value("plugins/enabled",  "").toStringList();
    QStringList panelPlugins   = this->settings->value("plugins/on_panel", "").toStringList();

    foreach(QString p, plugins) {
        // change to plugin directory
        this->pluginsDir.cd(p);
        Plugin *plugin = new Plugin(this->pluginsDir);
        this->pluginsDir.cdUp();
        if (plugin->isValid()) {
            QString pluginName = plugin->getName();
            // add list item
            QTreeWidgetItem *i = new QTreeWidgetItem();
            i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
            // is enabled
            i->setText((int)LCol::isEnabled, "");
            i->setCheckState(
                (int)LCol::isEnabled,
                (Qt::CheckState)(2 * enabledPlugins.contains(pluginName, Qt::CaseInsensitive)) // (false, true) = (0, 2)
            );
            // plugin name
            i->setText((int)LCol::pluginName, pluginName);
            // is on panel
            i->setText((int)LCol::isOnPanel, "");
            i->setCheckState(
                (int)LCol::isOnPanel,
                (Qt::CheckState)(2 * panelPlugins.contains(pluginName, Qt::CaseInsensitive)) // (false, true) = (0, 2)
            );
            // author
            i->setText((int)LCol::author, plugin->getAuthor());
            // description
            i->setText((int)LCol::description, plugin->getDescription());
            items.append(i);
        }
    }
    return items;
}

// refresh view
void PluginListWindow::refreshList() {
    // clear list
    this->ui->pluginList->clear();
    // load plugins
    QList<QTreeWidgetItem *> plugins = this->loadPluginsList();
    this->ui->pluginList->insertTopLevelItems(0, plugins);
}

// dump changes to config
void PluginListWindow::save() {
    QList<QTreeWidgetItem *> plugins = this->ui->pluginList->findItems("*", Qt::MatchWildcard, (int)LCol::pluginName);
    QString name;
    bool isEnabled, isOnPanel;
    QStringList enabledPlugins, panelPlugins;
    foreach(QTreeWidgetItem *plugin, plugins) {
        name = plugin->text((int)LCol::pluginName);
        isEnabled = plugin->checkState((int)LCol::isEnabled);
        isOnPanel = plugin->checkState((int)LCol::isOnPanel);
        if (isEnabled) {
            enabledPlugins.append(name);
            if (isOnPanel) {
                panelPlugins.append(name);
            }
        }
    }

    this->settings->setValue("plugins/enabled",  enabledPlugins);
    this->settings->setValue("plugins/on_panel", panelPlugins);

    this->accept();
}
