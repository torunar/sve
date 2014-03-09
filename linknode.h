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
    LinkNode(QList<QDomElement> elementNodes, QDomDocument *xml, QWidget *parent = 0);
    ~LinkNode(){}

private:
    QPainter       *painter;
    QImage          buffer;
    QPen            pen;

    QVector<QPoint> line;

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // LINKNODE_H
