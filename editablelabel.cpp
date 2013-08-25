#include "editablelabel.h"

EditableLabel::EditableLabel(QWidget *parent) : QLabel(parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("EditableLabel");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

EditableLabel::EditableLabel(const QString text, QWidget *parent) : QLabel(text, parent) {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setObjectName("EditableLabel");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
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
    //performDrag(ev->pos());
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
