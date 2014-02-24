#include "plugin.h"

QVector<QString> Plugin::getElementAttributes(QString element, QString attribute){
    QVector<QString> values;
    QDomNodeList elements = this->node.elementsByTagName(element);
    for (int i = 0; i < elements.size(); i++) {
        values.append(elements.at(i).toElement().attribute(attribute));
    }
    return values;
}

Plugin::Plugin(QString filename){
    QFile fileIn(filename);
    fileIn.open(QFile::ReadOnly);
    QString file = fileIn.readAll();
    fileIn.close();
    QString erm;
    int l, c;
    QDomDocument *xml = new QDomDocument();
    if (!xml->setContent(file, &erm, &l, &c)) {
        qDebug() << erm << ':' << l << '-' << c;
    }
    this->node = xml->firstChildElement("plugin");
}

Plugin::Plugin(QDir directory){
    qDebug() << "Loading plugin from" << directory.absolutePath();
    QString xmlPath = directory.absolutePath() + QDir::separator() + "plugin.xml";
    QString imgPath = directory.absolutePath() + QDir::separator() + "plugin.svg";
    // load xml
    if (QFile::exists(xmlPath)) {
        QFile fileIn(xmlPath);
        fileIn.open(QFile::ReadOnly);
        QString file = fileIn.readAll();
        fileIn.close();
        QString erm;
        int l, c;
        QDomDocument *xml = new QDomDocument();
        if (!xml->setContent(file, &erm, &l, &c)) {
            qDebug() << erm << ':' << l << '-' << c;
        }
        this->node = xml->firstChildElement("plugin");
    }
    else {
        qDebug() << "Plugin not found";
    }
    // load pixmap
    if (QFile::exists(imgPath)) {
        this->pixmap = QPixmap(imgPath);
    }
    else {
        qDebug() << "Pixmap not found";
    }
}

Plugin::Plugin(QDomElement node){
    this->node = node;
}

bool Plugin::isValid() {
    return !(this->node.isNull() || this->pixmap.isNull());
}

QString Plugin::getName() {
    return this->node.attribute("name");
}

QString Plugin::getAuthor() {
    return this->getElementAttributes("info", "author").first();
}

QString Plugin::getDescription() {
    return this->getElementAttributes("info", "description").first();
}

QVector<QString> Plugin::getIns(){
    return this->getElementAttributes("in", "name");
}

QVector<QString> Plugin::getOuts(){
    return this->getElementAttributes("out", "name");
}

QString Plugin::getSource(){
    return this->node.firstChildElement("src").text();
}

QPixmap Plugin::getPixmap(QSize size){
    return this->pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QList<Plugin *> Plugin::loadByList(QDir pluginsDir, QStringList filterPlugins) {
    QList<Plugin *> plugins;
    QStringList pluginList = pluginsDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

    foreach(QString p, pluginList) {
        // change to plugin directory
        pluginsDir.cd(p);
        Plugin *plugin = new Plugin(pluginsDir);
        if (filterPlugins.contains(plugin->getName(), Qt::CaseInsensitive)) {
            if (plugin->isValid()) {
                plugins.append(plugin);
            }
            else {
                qDebug() << "Plugin" << plugin->getName() << "seems broken";
            }
        }
        pluginsDir.cdUp();
    }
    return plugins;
}
