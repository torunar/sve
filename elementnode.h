#ifndef ELEMENTNODE_H
#define ELEMENTNODE_H

#include "plugin.h"
#include "unode.h"
#include "nodepropertiesdialog.h"
#include <QSettings>

#include <QDebug>

class ElementNode : public UNode
{
    Q_OBJECT

public:
    ElementNode(                                                        QWidget *parent = 0);
    ElementNode(                     Plugin *plugin, QDomDocument *xml, QWidget* parent = 0);
    ElementNode(const QDomNode node, Plugin *plugin, QDomDocument *xml, QWidget *parent = 0);
    ~ElementNode(){}
    Plugin *getPlugin();
    QString getName();
    QString getInput(int i);
    QString getOutput(int i);

protected:

private:
    Plugin    *plugin;
    QSettings *settings;

signals:

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void edit();
};

#endif // ELEMENTNODE_H
