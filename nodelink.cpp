#include "nodelink.h"

NodeLink::NodeLink(QWidget *parent) : EditableLabel(parent) {
    this->painter = new QPainter();
    this->pen = QPen(Qt::black,1,Qt::SolidLine);
    this->setGeometry(10,10,301,301);
    this->show();
}

void NodeLink::paintEvent(QPaintEvent *) {
    this->painter->begin(this);
    this->painter->setPen(this->pen);
    QVector<QPoint> line;
    line
        << QPoint(0,                 0)
        << QPoint(this->width() / 2, 0)
        << QPoint(this->width() / 2, this->height()-1)
        << QPoint(this->width(),     this->height()-1);
    this->painter->drawPolyline(line);
    this->painter->end();
}
