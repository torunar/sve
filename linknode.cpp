#include "linknode.h"

LinkNode::LinkNode(QList<UNode*> elementNodes, QDomDocument *xml, QWidget *parent) : UNode(parent) {
    this->setObjectName("link_node");
    disconnect(this, SIGNAL(customContextMenuRequested(const QPoint&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    this->settings = new QSettings("mike-schekotov", "sve");

    this->nodes = elementNodes;

    this->xml = xml;
    this->node = this->xml->createElement("link");
    this->node.setAttribute("id",    QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("first_id", elementNodes.first()->node.attribute("id"));
    this->node.setAttribute("last_id",  elementNodes.last()->node.attribute("id"));
    this->xml->firstChild().appendChild(this->node);

    this->painter = new QPainter();
    this->pen = QPen(Qt::black, 1, Qt::SolidLine);

    this->show();
}

bool LinkNode::hasNode(QString nodeID) {
    return (this->nodes.first()->node.attribute("id") == nodeID || this->nodes.last()->node.attribute("id") == nodeID);
}

void LinkNode::paintEvent(QPaintEvent *) {
    QSize nodeSize      = this->settings->value("default_doc/node_size").toSize();
    QPoint topLeft(
        std::min(
            this->nodes.first()->node.attribute("x").toInt() + nodeSize.width(),
            this->nodes.last()->node.attribute("x").toInt()
        ),
        std::min(
            this->nodes.first()->node.attribute("y").toInt() + nodeSize.height() / 2,
            this->nodes.last()->node.attribute("y").toInt() + nodeSize.height() / 2
        )
    );
    QPoint bottomRight(
        std::max(
            this->nodes.first()->node.attribute("x").toInt() + nodeSize.width(),
            this->nodes.last()->node.attribute("x").toInt()
        ),
        std::max(
            this->nodes.first()->node.attribute("y").toInt() + nodeSize.height() / 2,
            this->nodes.last()->node.attribute("y").toInt() + nodeSize.height() / 2
        )
    );

    QPoint p1(nodes.first()->pos().x() - topLeft.x() + nodeSize.width(), nodes.first()->pos().y() - topLeft.y() + nodeSize.height() / 2);
    QPoint p2(nodes.last()->pos().x()  - topLeft.x()                   , nodes.last()->pos().y()  - topLeft.y() + nodeSize.height() / 2);

    bool lineType = (p1.x() < p2.x());
    line.clear();
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
    this->painter->end();
}

void LinkNode::mouseMoveEvent(QMouseEvent *ev) {
}

void LinkNode::showContextMenu(const QPoint &p) {
    QVector<QPoint> other;
    other << QPoint(p.x()-2, p.y()-2) << QPoint(p.x()+2, p.y()-2) << QPoint(p.x()+2, p.y()+2) << QPoint(p.x()-2, p.y()-2);
    qDebug() << QPolygon(line).containsPoint(p, Qt::OddEvenFill) << QPolygon(line).isSharedWith(other);

//    QPixmap t = QScreen::grabWindow(this->winId(), QCursor::pos().x() -5, QCursor::pos().y()-5, 10, 10);
//    qDebug() << t.colorCount();
    /*
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu menu;
    QAction *edit = new QAction(tr("Edit text"), this);
            connect(edit, SIGNAL(triggered()), this, SLOT(edit()));
            menu.addAction(edit);
    QAction *del = new QAction(tr("Delete"), this);
            connect(del, SIGNAL(triggered()), this, SLOT(remove()));
            menu.addAction(del);
    menu.exec(globalPos);
    */
}
