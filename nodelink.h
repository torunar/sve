#ifndef NODELINK_H
#define NODELINK_H

#include <QPoint>
#include <QLine>
#include <QPolygon>
#include <QVector>

#include <QDebug>

class NodeLink
{
public:
    NodeLink();
    void setBegin(QPoint point);
    void setEnd(QPoint point);
    void setMiddle(QPoint point);
    QVector<QPoint> getPoints();

private:
    QVector<QPoint> points;
};

#endif // NODELINK_H
