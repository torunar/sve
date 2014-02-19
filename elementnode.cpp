#include "elementnode.h"

ElementNode::ElementNode(QWidget *parent) : EditableLabel(parent) {
    this->settings = new QSettings("torunar", "sve");
    this->settings->beginGroup("default_doc");

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    QDir d("../plugins/and/");
    Plugin *p = new Plugin(d);

    QPixmap background = p->getPixmap(this->settings->value("node_size").toSize());
    this->setPixmap(background);
    this->show();
}

void ElementNode::setProperties(NodePropertiesWindow *window) {
    QVector<QString> *vIn = new QVector<QString>;
    vIn->append("in-1");
    vIn->append("in-2");
    vIn->append("in-3");
    window->setInputs(vIn);
    QVector<QString> *vOut = new QVector<QString>;
    vOut->append("out-1");
    vOut->append("out-2");
    vOut->append("out-3");
    window->setOutputs(vOut);
    window->setSource("{\n\tlol;\n}");
}

void ElementNode::showContextMenu(const QPoint &pos) {
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu menu;
    QAction *edit = new QAction(tr("Edit properties"), this);
            connect(edit, SIGNAL(triggered()), this, SLOT(editNode()));
            menu.addAction(edit);
    QAction *del = new QAction(tr("Delete"), this);
            connect(del, SIGNAL(triggered()), this, SLOT(deleteNode()));
            menu.addAction(del);
            menu.exec(globalPos);
}

void ElementNode::editNode() {
    NodePropertiesWindow *np = new NodePropertiesWindow();
    this->setProperties(np);
    np->show();
    qDebug() << "edit, ha";
}

void ElementNode::deleteNode() {
    qDebug() << "delete, ha";
    delete(this);
}
