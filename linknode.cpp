#include "linknode.h"

LinkNode::LinkNode(QList<UNode*> elementNodes, QDomDocument *xml, QWidget *parent) : UNode(parent) {
    this->setObjectName("link_node");
    this->settings = new QSettings("mike-schekotov", "sve");

    this->nodes = elementNodes;

    this->xml = xml;
    this->node = this->xml->createElement("link");
    this->node.setAttribute("id",    QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("first_id", elementNodes.first()->node.attribute("id"));
    this->node.setAttribute("last_id",  elementNodes.last()->node.attribute("id"));
    this->xml->firstChild().appendChild(this->node);

    this->painter = new QPainter();
    this->pen = QPen(Qt::black,1,Qt::SolidLine);
    this->show();
}

bool LinkNode::hasNode(QString nodeID) {
    return (this->nodes.first()->node.attribute("id") == nodeID || this->nodes.last()->node.attribute("id") == nodeID);
}

void LinkNode::paintEvent(QPaintEvent *) {
    QSize nodeSize      = this->settings->value("default_doc/node_size").toSize();
    QPoint topLeft(
        this->nodes.first()->node.attribute("x").toInt() + nodeSize.width(),
        this->nodes.first()->node.attribute("y").toInt() + nodeSize.height() / 2
    );
    QPoint bottomRight(
        this->nodes.last()->node.attribute("x").toInt(),
        this->nodes.last()->node.attribute("y").toInt() + nodeSize.height() / 2
    );
    if (topLeft.x() > bottomRight.x()) {
        int x = topLeft.x();
        topLeft.setX(bottomRight.x());
        bottomRight.setX(x);
    }
    if (topLeft.y() > bottomRight.y()) {
        int y = topLeft.y();
        topLeft.setY(bottomRight.y());
        bottomRight.setY(y);
    }

    QPoint p1(nodes.first()->pos().x() - topLeft.x() + nodeSize.width(), nodes.first()->pos().y() - topLeft.y() + nodeSize.height() / 2);
    QPoint p2(nodes.last()->pos().x()  - topLeft.x()                   , nodes.last()->pos().y()  - topLeft.y() + nodeSize.height() / 2);

    bool lineType = (p1.x() < p2.x());
    if(lineType) {
        this->setGeometry(QRect(topLeft, bottomRight));
        line << QPoint(p1.x(),            p1.y())
             << QPoint(this->width() / 2, p1.y())
             << QPoint(this->width() / 2, p2.y())
             << QPoint(p2.x(),            p2.y());
    }
    else {
        this->setGeometry(topLeft.x()-10, topLeft.y(), bottomRight.x() - topLeft.x() + 21, bottomRight.y() - topLeft.y() + 1);
        line << QPoint(p1.x() + 10,       p1.y())
             << QPoint(p1.x() + 20,       p1.y())
             << QPoint(p1.x() + 20,       this->height() / 2)
             << QPoint(p2.x(),            this->height() / 2)
             << QPoint(p2.x(),            p2.y())
             << QPoint(p2.x() + 10,       p2.y());
    }

    this->painter->begin(this);
    this->painter->setPen(this->pen);
    this->painter->drawPolyline(line);
    line.clear();
    this->painter->end();
}

void LinkNode::mouseMoveEvent(QMouseEvent *ev) {
}
