#ifndef LABELNODE_H
#define LABELNODE_H

#include <QInputDialog>
#include <QMdiSubWindow>
#include "unode.h"

#include <QDebug>

class LabelNode : public UNode
{
    Q_OBJECT
public:
    LabelNode(QWidget *parent = 0);
    LabelNode(const QString  text, QDomDocument *xml, QWidget *parent = 0);
    LabelNode(const QDomNode node, QDomDocument *xml, QWidget *parent = 0);
    ~LabelNode(){}

protected:

private:
    
signals:

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void edit();
};

#endif // LABELNODE_H
