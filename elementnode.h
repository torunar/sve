#ifndef ELEMENTNODE_H
#define ELEMENTNODE_H

#include <QPixmap>
#include <editablelabel.h>

#include <QDebug>
#include "nodepropertieswindow.h"

class ElementNode : public EditableLabel
{
    Q_OBJECT
public:
    ElementNode(QWidget *parent = 0);
    void setProperties(NodePropertiesWindow *window);

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void editNode();
    void deleteNode();
};

#endif // ELEMENTNODE_H
