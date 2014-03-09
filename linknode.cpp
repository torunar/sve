#include "linknode.h"

LinkNode::LinkNode(QList<QDomElement> elementNodes, QDomDocument *xml, QWidget *parent) : UNode(parent) {
    QSettings *settings = new QSettings("torunar", "sve");
    QSize nodeSize     = settings->value("default_doc/node_size").toSize();
    QPoint topLeft(
        elementNodes.first().attribute("x").toInt() + nodeSize.width(),
        elementNodes.first().attribute("y").toInt() + nodeSize.height() / 2
    );
    QPoint bottomRight(
        elementNodes.last().attribute("x").toInt(),
        elementNodes.last().attribute("y").toInt() + nodeSize.height() / 2
    );

    this->xml = xml;
    this->node = this->xml->createElement("link");
    this->node.setAttribute("id",    QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("first", elementNodes.first().attribute("id"));
    this->node.setAttribute("last",  elementNodes.last().attribute("id"));
    this->xml->firstChild().appendChild(this->node);

    this->painter = new QPainter();
    this->pen = QPen(Qt::black,1,Qt::SolidLine);
    this->setGeometry(QRect(topLeft, bottomRight));
    this->show();
}

void LinkNode::paintEvent(QPaintEvent *) {
    this->painter->begin(this);
    this->painter->setPen(this->pen);
    line
        << QPoint(0,                 0)
        << QPoint(this->width() / 2, 0)
        << QPoint(this->width() / 2, this->height()-1)
        << QPoint(this->width(),     this->height()-1);
    this->painter->drawPolyline(line);
    line.clear();
    this->painter->end();
}
