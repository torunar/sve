#include "linknode.h"

LinkNode::LinkNode(QList<UNode*> elementNodes, QPair<int, int> connectors, QDomDocument *xml, QWidget *parent) : UNode(parent) {
    this->setObjectName("link_node");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)), Qt::UniqueConnection);

    this->settings   = new QSettings("mike-schekotov", "sve");

    this->nodes      = elementNodes;
    this->connectors = connectors;

    this->xml = xml;
    this->node = this->xml->createElement("link");
    this->node.setAttribute("id",              QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("first_id",        elementNodes.first()->attr("id"));
    this->node.setAttribute("last_id",         elementNodes.last()->attr("id"));
    this->node.setAttribute("first_connector", connectors.first);
    this->node.setAttribute("last_connector",  connectors.second);
    this->xml->firstChild().appendChild(this->node);

    this->painter = new QPainter();
    this->pen = QPen(Qt::black, 1, Qt::SolidLine);

    this->show();
}

bool LinkNode::hasNode(QString nodeID) {
    return (this->nodes.first()->attr("id") == nodeID || this->nodes.last()->attr("id") == nodeID);
}

void LinkNode::paintEvent(QPaintEvent *) {
    QSize nodeSize      = this->settings->value("default_doc/node_size").toSize();
    ElementNode *firstNode = (ElementNode*) this->nodes.first();
    ElementNode *lastNode  = (ElementNode*) this->nodes.last();

    int nOuts  = ((ElementNode*) firstNode)->getPlugin()->getOutputs().size();
    int nIns   = ((ElementNode*) lastNode)->getPlugin()->getInputs().size();

    // geometry tricks
    int xFirst = firstNode->attr("x").toInt();
    int xLast  = lastNode->attr("x").toInt();

    int yFirst = firstNode->attr("y").toInt();
    int yLast  = lastNode->attr("y").toInt();

    int marginFirst = (this->connectors.first  + 1) * nodeSize.height() / (nOuts + 1);
    int marginLast  = (this->connectors.second + 1) * nodeSize.height() / (nIns  + 1);

    QPoint topLeft(
        std::min(xFirst + nodeSize.width(), xLast),
        std::min(yFirst + marginFirst,      yLast  + marginLast)
    );
    QPoint bottomRight(
        std::max(xFirst + nodeSize.width(), xLast),
        std::max(yFirst + marginFirst,      yLast  + marginLast)
    );

    QPoint p1(firstNode->pos().x() - topLeft.x() + nodeSize.width(), firstNode->pos().y() - topLeft.y() + marginFirst);
    QPoint p2(lastNode->pos().x()  - topLeft.x()                   , lastNode->pos().y()  - topLeft.y() + marginLast);

    line.clear();

    // shape of line
    bool lineType = (p1.x() < p2.x());
    if (lineType) {
        this->setGeometry(topLeft.x(), topLeft.y() - 1, bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y() + 3);
        int fix = (p1.y() == 0) ? 1 : -1;
        line << QPoint(p1.x(),            p1.y() + fix)
             << QPoint(this->width() / 2, p1.y() + fix)
             << QPoint(this->width() / 2, abs(p2.y() - fix))
             << QPoint(p2.x(),            abs(p2.y() - fix));
    }
    else {
        this->setGeometry(topLeft.x()-11, topLeft.y()-1, bottomRight.x() - topLeft.x() + 21, bottomRight.y() - topLeft.y() + 1);
        int fixY = (p1.y() == 0) ? 1 : -1;
        int fixX = (p1.x() == 0) ? 1 : -1;
        line << QPoint(p1.x() + 10,        p1.y() + fixY)
             << QPoint(p1.x() + 20 + fixX, p1.y() + fixY)
             << QPoint(p1.x() + 20 + fixX, this->height() / 2)
             << QPoint(p2.x() - fixX,      this->height() / 2)
             << QPoint(p2.x() - fixX,      p2.y() - fixY)
             << QPoint(p2.x() + 10,        p2.y() - fixY);
    }

    // mask to crop the widget
    QRegion mask;
    for (int i = 0; i < line.size() - 1; i++) {
        QPoint xy1 = line[i];
        QPoint xy2 = line[i+1];
        if (xy1.y() == xy2.y()) {
            xy1.setY(xy2.y() - 1);
            xy2.setY(xy2.y() + 1);
        }
        else {
            xy1.setX(xy1.x() - 1);
            xy2.setX(xy2.x() + 1);
        }
        topLeft = QPoint(
            std::min(xy1.x(), xy2.x()),
            std::min(xy1.y(), xy2.y())
        );
        bottomRight = QPoint(
            std::max(xy1.x(), xy2.x()),
            std::max(xy1.y(), xy2.y())
        );
        mask += QRegion(QRect(topLeft, bottomRight));
    }

    this->painter->begin(this);
    this->painter->setPen(this->pen);
    this->painter->drawPolyline(line);
    this->setMask(mask);
    this->painter->end();
}

void LinkNode::mouseMoveEvent(QMouseEvent *) {
}

void LinkNode::showContextMenu(const QPoint &pos) {
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu menu;
    QAction *edit = new QAction(tr("Edit connectors"), this);
            connect(edit, SIGNAL(triggered()), this, SLOT(edit()));
            menu.addAction(edit);
    QAction *del = new QAction(tr("Delete"), this);
            connect(del, SIGNAL(triggered()), this, SLOT(remove()));
            menu.addAction(del);
            menu.exec(globalPos);
}

void LinkNode::edit() {
    emit(showConnectionDialog(this->nodes));
}
