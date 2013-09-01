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

    this->node = xml->createElement("label");
    /*
    QDomElement name = xml->createElement("name");
    QDomText txt = xml->createTextNode(QString::number(QDateTime::currentMSecsSinceEpoch()));
    name.appendChild(txt);
    this->node.appendChild(name);
    QDomElement x = xml->createElement("x");
    txt = xml->createTextNode("0");
    x.appendChild(txt);
    this->node.appendChild(x);
    QDomElement y = xml->createElement("y");
    txt = xml->createTextNode("0");
    y.appendChild(txt);
    this->node.appendChild(y);
    */
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
        this->node.setAttribute("text", text);
    }
}

void EditableLabel::deleteLabel() {
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
