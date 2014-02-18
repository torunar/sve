#ifndef PLUGIN_H
#define PLUGIN_H

#include <QVector>
#include <QtXml/QDomElement>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QPixmap>

class Plugin
{
private:
    QDomElement node;
    QPixmap pixmap;
    QVector<QString> getElementAttributes(QString element, QString attribute);

public:
    Plugin(QString filename);   // load from file
    Plugin(QDir directory);     // load from path
    Plugin(QDomElement node);   // load from proto
    bool isValid();
    QString getName();
    QString getAuthor();
    QString getDescription();
    QVector<QString> getIns();
    QVector<QString> getOuts();
    QString getSource();
    QPixmap getPixmap(QSize size);
};

#endif // PLUGIN_H
