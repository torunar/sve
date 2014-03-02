#include "nodelink.h"

NodeLink::NodeLink(){
    this->points = QVector<QPoint>(4, QPoint(0, 0));
}

void NodeLink::setBegin(QPoint point){
    this->points[0] = point;
}

void NodeLink::setEnd(QPoint point){
    this->points[3] = point;
}

void NodeLink::setMiddle(QPoint point){
    this->points[1] = QPoint(point.x(), this->points[0].y());
    this->points[2] = QPoint(point.x(), this->points[3].y());
}

QVector<QPoint> NodeLink::getPoints(){
    return this->points;
}
