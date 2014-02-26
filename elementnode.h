#ifndef ELEMENTNODE_H
#define ELEMENTNODE_H

#include "plugin.h"
#include "editablelabel.h"
#include <QSettings>

#include <QDebug>
#include "nodepropertieswindow.h"

class ElementNode : public EditableLabel
{
    Q_OBJECT
public:
    ElementNode(QWidget *parent = 0);
    ElementNode(Plugin *plugin, QDomDocument *xml, QWidget* parent = 0);
    ElementNode(const QDomNode node, Plugin *plugin, QDomDocument *xml, QWidget *parent = 0);
    void setProperties(NodePropertiesWindow *window);

private:
    QSettings *settings;
    QPoint startPos;

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void edit();
};

#endif // ELEMENTNODE_H
