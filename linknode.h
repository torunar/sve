#ifndef LINKNODE_H
#define LINKNODE_H

#include <QImage>
#include <QPainter>
#include <QSettings>
#include "unode.h"

class LinkNode : public UNode
{
    Q_OBJECT
public:
    LinkNode(QList<UNode*> elementNodes, QDomDocument *xml, QWidget *parent = 0);
    ~LinkNode(){}
    bool hasNode(QString nodeID);

private:
    QPainter       *painter;
    QImage          buffer;
    QPen            pen;
    QSettings      *settings;

    QList<UNode *>  nodes;
    QVector<QPoint> line;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *ev);
//    void mousePressEvent(QMouseEvent *ev);

signals:

public slots:
    void showContextMenu(const QPoint&);
};

#endif // LINKNODE_H
