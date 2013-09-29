#include "editablelabel.h"

EditableLabel::EditableLabel(const QString text, QDomDocument *xml, QWidget *parent) : QLabel(text, parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("EditableLabel");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    this->xml = xml;

    this->node = xml->createElement("label");
    this->node.setAttribute("name", QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("x", 0);
    this->node.setAttribute("y", 0);
    this->node.setAttribute("text", text);
    xml->appendChild(this->node);
}

void EditableLabel::showContextMenu(const QPoint& pos) {
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu menu;
    QAction *edit = new QAction(tr("Edit text"), this);
            connect(edit, SIGNAL(triggered()), this, SLOT(editText()));
            menu.addAction(edit);
    QAction *del = new QAction(tr("Delete"), this);
            connect(del, SIGNAL(triggered()), this, SLOT(deleteLabel()));
            menu.addAction(del);
    menu.exec(globalPos);
}

void EditableLabel::editText() {
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

void EditableLabel::deleteLabel() {
    // remove from xml
    this->xml->removeChild(this->node);
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

void EditableLabel::mouseReleaseEvent(QMouseEvent *ev) {
    this->node.setAttribute("x", this->x());
    this->node.setAttribute("y", this->y());
    // document changed flag
    emit altered(1);
}

void EditableLabel::performDrag(const QPoint endPos) {
    int newX = (this->x() + (endPos - startPos).x()) / 10 * 10 - 1;
    int newY = (this->y() + (endPos - startPos).y()) / 10 * 10 - 1;
    this->setGeometry(
        (newX >= 0) ? newX : 0,
        (newY >= 0) ? newY : 0,
        this->width(),
        this->height()
    );
}
