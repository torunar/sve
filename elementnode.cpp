#include "elementnode.h"

ElementNode::ElementNode(QWidget *parent) : EditableLabel(parent) {
    // stub
}

ElementNode::ElementNode(Plugin *plugin, QDomDocument *xml, QWidget *parent) : EditableLabel(parent) {
    this->settings = new QSettings("torunar", "sve");

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    this->xml = xml;
    this->plugin = plugin;

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

}

ElementNode::ElementNode(const QDomNode node, Plugin *plugin, QDomDocument *xml, QWidget *parent) : EditableLabel(parent) {
    this->settings = new QSettings("torunar", "sve");

    this->xml = xml;
    this->node = node.toElement();
    this->plugin = plugin;

    QSize pixmapSize = this->settings->value("default_doc/node_size").toSize();
    this->setPixmap(plugin->getPixmap(pixmapSize));

    this->show();
    // move
    this->setPosition(this->node.attribute("x").toInt(), this->node.attribute("y").toInt());
    this->startPos = QPoint(0, 0);
}

void ElementNode::setCounters(uint inCounter, uint outCounter) {
    this->inCounter  = inCounter;
    this->outCounter = outCounter;
}

void ElementNode::showContextMenu(const QPoint &pos) {
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu menu;
    QAction *edit = new QAction(tr("Edit properties"), this);
            connect(edit, SIGNAL(triggered()), this, SLOT(edit()));
            menu.addAction(edit);
    QAction *del = new QAction(tr("Delete"), this);
            connect(del, SIGNAL(triggered()), this, SLOT(remove()));
            menu.addAction(del);
            menu.exec(globalPos);
}

void ElementNode::edit() {
    NodePropertiesWindow *np = new NodePropertiesWindow();
    np->setCounters(this->inCounter, this->outCounter);
    np->setInputs(this->plugin->getInputs());
    np->setOutputs(this->plugin->getOutputs());
    np->setSource(this->plugin->getSource());
    np->show();
//    qDebug() << "edit, ha";
}
