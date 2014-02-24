#include "elementnode.h"

ElementNode::ElementNode(QWidget *parent) : EditableLabel(parent) {
    // stub
}

ElementNode::ElementNode(Plugin *plugin, QDomDocument *xml, QWidget *parent) : EditableLabel(parent) {
    this->settings = new QSettings("torunar", "sve");

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    this->xml = xml;

    this->node = this->xml->createElement("node");
    // set attributes
    this->node.setAttribute("id", QString::number(QDateTime::currentMSecsSinceEpoch()));
    this->node.setAttribute("x", 0);
    this->node.setAttribute("y", 0);
    this->node.setAttribute("plugin", plugin->getName());
    // append to root node
    this->xml->firstChild().appendChild(this->node);

    QSize pixmapSize = this->settings->value("default_doc/node_size").toSize();
    this->setPixmap(plugin->getPixmap(pixmapSize));
    this->show();

    qDebug() << this->xml->toString();
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
