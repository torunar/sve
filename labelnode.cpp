#include "labelnode.h"

LabelNode::LabelNode(QWidget *parent) : UNode(parent) {
    // stub
}

LabelNode::LabelNode(const QString text, QDomDocument *xml, QWidget *parent) : UNode(text, parent) {
    this->setObjectName("label_node");
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

LabelNode::LabelNode(const QDomNode node, QDomDocument *xml, QWidget *parent) : UNode(node.toElement().attribute("text"), parent) {
    this->setObjectName("label_node");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    this->xml = xml;
    this->node = node.toElement();
    this->show();
    // move
    this->setPosition(this->attr("x").toInt(), this->attr("y").toInt());
    this->startPos = QPoint(0, 0);
}

void LabelNode::showContextMenu(const QPoint& pos) {
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

void LabelNode::edit() {
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
        emit altered(AlterType::Edited);
    }
}
