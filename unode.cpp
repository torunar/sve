#include "unode.h"

UNode::UNode(QWidget *parent) : QLabel(parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

UNode::UNode(QString text, QWidget *parent) : QLabel(text, parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

UNode::UNode(const UNode &unode) : QLabel() {
    this->node     = unode.node;
    this->xml      = unode.xml;
    this->startPos = unode.startPos;
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

QString UNode::getID() {
    return this->node.attribute("id");
}

void UNode::setPosition(int x, int y) {
    this->setGeometry(
        (x >= 0) ? x : 0,
        (y >= 0) ? y : 0,
        this->width(),
        this->height()
    );
    this->node.setAttribute("x", x);
    this->node.setAttribute("y", y);
}

void UNode::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        this->startPos = ev->pos();
        emit(activated());
    }
}

void UNode::mouseMoveEvent(QMouseEvent *ev) {
    if (ev->buttons() & Qt::LeftButton) {
        int distance = (ev->pos() - this->startPos).manhattanLength();
        if (distance >= 10)
            this->performDrag(ev->pos());
    }
}

void UNode::mouseReleaseEvent(QMouseEvent *) {
    this->node.setAttribute("x", this->x());
    this->node.setAttribute("y", this->y());
    // document changed flag
    emit altered(AlterType::Moved);
}

void UNode::performDrag(const QPoint endPos) {
    // grid positioning
    int newX = 10 * ((this->x() + (endPos - startPos).x()) / 10);
    int newY = 10 * ((this->y() + (endPos - startPos).y()) / 10);
    this->setGeometry(
        (newX >= 0) ? newX : 0,
        (newY >= 0) ? newY : 0,
        this->width(),
        this->height()
    );
}


// remove from xml
void UNode::remove() {
    this->xml->firstChild().removeChild(this->node);
    emit altered(AlterType::Deleted);
    delete(this);
}
