#include "linkwidget.h"

LinkWidget::LinkWidget(QWidget *parent) : QWidget(parent) {
    this->painter = new QPainter();
    this->pen = QPen(Qt::black,1,Qt::SolidLine);
}

void LinkWidget::paintEvent(QPaintEvent *) {
    this->painter->begin(this);
    this->painter->setPen(this->pen);
    NodeLink *nl = new NodeLink();
    nl->setBegin(QPoint(100, 100));
    nl->setEnd(QPoint(300, 300));
    nl->setMiddle(QPoint(200, 200));
    this->painter->drawPolyline(nl->getPoints());
    this->painter->end();
}
