#include "pluginlistwindow.h"
#include "ui_pluginlistwindow.h"

PluginListWindow::PluginListWindow(QWidget *parent) : QDialog(parent), ui(new Ui::PluginListWindow) {
    ui->setupUi(this);
    this->settings = new QSettings("torunar", "sve");
    this->settings->beginGroup("Plugins");

    connect(this->ui->refreshButton, SIGNAL(clicked()), this, SLOT(refreshList()));
    connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

PluginListWindow::~PluginListWindow() {
    delete ui;
}

void PluginListWindow::refreshList() {
    // plugins root
    QString path = settings->value("PluginDir", "./plugins/").toString();
    QDir dir(path);

    // possible plugins
    QStringList plugins = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

    // loading plugins
    this->ui->pluginList->clear();
    foreach(QString p, plugins) {
        // change to plugin directory
        dir.cd(p);
        Plugin *plugin = new Plugin(dir);
        dir.cdUp();
        if (plugin->isValid()) {
            // add list item
            QStringList row;
            row << p;
            QTreeWidgetItem *i = new QTreeWidgetItem();
            i->setFlags(i->flags() | Qt::ItemIsUserCheckable);
            // enabled
            i->setText(0, "");
            i->setCheckState(0, Qt::Unchecked);
            // plugin name
            i->setText(1, plugin->getName());
            // on panel
            i->setText(2, "");
            i->setCheckState(2, Qt::Unchecked);
            // author
            i->setText(3, plugin->getAuthor());
            // description
            i->setText(4, plugin->getDescription());
            this->ui->pluginList->insertTopLevelItem(0, i);
        }
    }
}
