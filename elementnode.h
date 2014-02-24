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
    void setProperties(NodePropertiesWindow *window);

private:
    QSettings *settings;

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void editNode();
    void deleteNode();
};

#endif // ELEMENTNODE_H
