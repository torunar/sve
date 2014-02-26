#include "editablelabel.h"

EditableLabel::EditableLabel(QWidget *parent) : QLabel(parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("EditableLabel");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

EditableLabel::EditableLabel(const QString text, QDomDocument *xml, QWidget *parent) : QLabel(text, parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("EditableLabel");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    this->xml = xml;
    // create via domdocument call
    this->node = xml->createElement("label");
    // set attributes
    this->node.setAttribute("id", QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("x", 0);
    this->node.setAttribute("y", 0);
    this->node.setAttribute("text", text);
    // append to root node
    xml->firstChild().appendChild(this->node);
    this->show();
    this->startPos = QPoint(0, 0);
}

EditableLabel::EditableLabel(const QDomNode node, QDomDocument *xml, QWidget *parent) : QLabel(node.toElement().attribute("text"), parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("EditableLabel");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    this->xml = xml;
    this->node = node.toElement();
    this->show();
    // move
    this->setPosition(this->node.attribute("x").toInt(), this->node.attribute("y").toInt());
    this->startPos = QPoint(0, 0);
}

void EditableLabel::showContextMenu(const QPoint& pos) {
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu menu;
    QAction *edit = new QAction(tr("Edit text"), this);
            connect(edit, SIGNAL(triggered()), this, SLOT(edit()));
            menu.addAction(edit);
    QAction *del = new QAction(tr("Delete"), this);
            connect(del, SIGNAL(triggered()), this, SLOT(remove()));
            menu.addAction(del);
    menu.exec(globalPos);
}

void EditableLabel::edit() {
    bool ok;
    QString text = QInputDialog::getText(this,
        tr("New label"),
        tr("Insert text for this label"),
        QLineEdit::Normal,
        this->text(),
        &ok
    );
    if (ok && !text.isEmpty()) {
        this->setText(text);
        this->adjustSize();
        this->node.setAttribute("text", text);
        // document changed flag
        emit altered(2);
    }
}

void EditableLabel::remove() {
    // remove from xml
    qDebug() << this->xml->toString();
    this->xml->firstChild().removeChild(this->node);
    qDebug() << this->xml->toString();
    emit altered(3);
    // remove from the world
    delete(this);
}

void EditableLabel::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        this->startPos = ev->pos();
    }
}

void EditableLabel::mouseMoveEvent(QMouseEvent *ev) {
    if (ev->buttons() & Qt::LeftButton) {
        int distance = (ev->pos() - this->startPos).manhattanLength();
        if (distance >= 10)
            this->performDrag(ev->pos());
    }
}

void EditableLabel::mouseReleaseEvent(QMouseEvent *) {
    this->node.setAttribute("x", this->x());
    this->node.setAttribute("y", this->y());
    // document changed flag
    emit altered(1);
}

void EditableLabel::performDrag(const QPoint endPos) {
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

void EditableLabel::setPosition(int x, int y) {
    this->setGeometry(
        (x >= 0) ? x : 0,
        (y >= 0) ? y : 0,
        this->width(),
        this->height()
    );
    this->node.setAttribute("x", x);
    this->node.setAttribute("y", y);
}
