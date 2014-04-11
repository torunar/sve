#ifndef ELEMENTNODE_H
#define ELEMENTNODE_H

#include "plugin.h"
#include "unode.h"
#include <QSettings>

#include <QDebug>
#include "nodepropertieswindow.h"

class ElementNode : public UNode
{
    Q_OBJECT

public:
    ElementNode(QWidget *parent = 0);
    ElementNode(                     Plugin *plugin, QDomDocument *xml, QWidget* parent = 0);
    ElementNode(const QDomNode node, Plugin *plugin, QDomDocument *xml, QWidget *parent = 0);
    ~ElementNode(){}
    void setCounters(uint inCounter, uint outCounter);
    Plugin *getPlugin();

protected:

private:
    Plugin    *plugin;
    QSettings *settings;
    uint       inCounter, outCounter;

signals:

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void edit();
};

#endif // ELEMENTNODE_H
