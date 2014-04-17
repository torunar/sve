#include "document.h"

/* main constructor */
Document::Document(QMdiSubWindow *parent) {
    this->parent = parent;
    this->setObjectName("document");

    // scroll area inside of subwindow
    this->container = new QScrollArea(this->parent);

    // finally, workarea
    this->workarea = new QFrame(this->container);
    this->workarea->setObjectName("document_area");
    this->workarea->setFrameStyle(QFrame::StyledPanel);

    this->container->setWidget(workarea);

    if (parent) this->parent->setWidget(this->container);

    this->title         = "";
    this->changed       = false;
    this->inCounter     = 0;
    this->outCounter    = 0;
    this->mode          = DocumentMode::Default;
    this->nodeCounter   = 0;
    this->activeElement = 0;

    this->xml = new QDomDocument("SVE");
    this->xml->appendChild(this->xml->createElement("document"));

    this->pushToHistory();
}

/* resize workarea */
void Document::resize(const int w, const int h) {
    this->workarea->resize(w, h);
}

/* resize workarea */
void Document::resize(const QSize size) {
    this->workarea->resize(size);
    this->xml->firstChild().toElement().setAttribute("width",  size.width());
    this->xml->firstChild().toElement().setAttribute("height", size.height());
}

/* embed into DocWindow */
void Document::attachToWindow(QMdiSubWindow *parent) {
    this->parent = parent;
    this->container->setParent(this->parent);
    this->parent->setWidget(this->container);
}

/* get size of workarea */
QSize Document::getSize() {
    return this->workarea->size();
}

/* check if document tree was changed */
bool Document::isChanged() {
    return this->changed;
}

/* add label to the document and xml tree */
void Document::addLabel(const QString text, bool skipHistory) {
    if (!skipHistory) {
        this->pushToHistory();
    }
    LabelNode *label = new LabelNode(text, this->xml, this->workarea);
    connect(label, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    this->setChanged(true);
}

/* add label from xml node */
void Document::addLabel(const QDomNode node, bool skipHistory) {
    if (!skipHistory) {
        this->pushToHistory();
    }
    LabelNode *label = new LabelNode(node, this->xml, this->workarea);
    connect(label, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    this->setChanged(true);
}

/* add node */
void Document::addNode(Plugin *plugin, bool skipHistory) {
    if (!skipHistory) {
        this->pushToHistory();
    }
    ElementNode *elementNode = new ElementNode(plugin, this->xml, this->workarea);
    connect(elementNode, SIGNAL(activated()),        this, SLOT(setActiveElement()));
    connect(elementNode, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    // update internal counters
    elementNode->setCounters(inCounter, outCounter);
    this->inCounter  += plugin->getInputs().size();
    this->outCounter += plugin->getOutputs().size();
    // set changed flag
    this->setChanged(true);
}

/* add node by xml node */
void Document::addNode(const QDomNode node, bool skipHistory){
    if (!skipHistory) {
        this->pushToHistory();
    }
    QString pluginName = node.toElement().attribute("plugin");
    Plugin *plugin = this->getPlugin(pluginName);
    ElementNode *elementNode = new ElementNode(node, plugin, this->xml, this->workarea);
    connect(elementNode, SIGNAL(activated()),        this, SLOT(setActiveElement()));
    connect(elementNode, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    // update internal counters
    elementNode->setCounters(inCounter, outCounter);
    this->inCounter  += plugin->getInputs().size();
    this->outCounter += plugin->getOutputs().size();
    // set changed flag
    this->setChanged(true);
}

/* add node by plugin name */
void Document::addNode(QString plugin){
    this->addNode(this->getPlugin(plugin));
}

/* add link */
void Document::addLink(QList<UNode*> elementNodes, QPair<int, int> connectors, bool skipHistory) {
    if (!skipHistory) {
        this->pushToHistory();
    }
    LinkNode *linkNode = new LinkNode(elementNodes, connectors, this->xml, this->workarea);
    connect(linkNode, SIGNAL(activated()),        this, SLOT(setActiveElement()));
    connect(linkNode, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    // reset document mode
    this->setMode(DocumentMode::Default);
    this->setChanged(true);
}

/* add link by xml node */
void Document::addLink(const QDomNode node, bool skipHistory) {
    if (!skipHistory) {
        this->pushToHistory();
    }

    QList<UNode *> elementNodes;
    elementNodes << this->getNodeByID(node.toElement().attribute("first_id"));
    elementNodes << this->getNodeByID(node.toElement().attribute("last_id"));

    LinkNode *linkNode = new LinkNode(node, elementNodes, this->xml, this->workarea);
    connect(linkNode, SIGNAL(activated()),        this, SLOT(setActiveElement()));
    connect(linkNode, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    this->setChanged(true);
}

/* get node by xml node's id */
UNode* Document::getNodeByID(QString id) {
    QString type;
    foreach(QObject *item, this->workarea->children()) {
        type = item->objectName();
        if ((type == "link_node") || (type == "element_node") || (type == "label_node")) {
            if (((UNode*) item)->attr("id") == id) {
                return (UNode*) item;
            }
        }
    }
    return NULL;
}

/* undo last action */
void Document::undo(){
    if(!this->history.empty()) {
        QByteArray state = this->history.pop();
        this->xml->setContent(state);
        this->renderNodes();
    };
}

/* save document to file */
void Document::save(QString filename) {
    this->filename = filename;
    this->title = QFileInfo(filename).baseName() + ".sve";
    QFile fileOut(this->filename);
    fileOut.open(QFile::WriteOnly);
    fileOut.write(this->xml->toString().toUtf8());//.toAscii());
    fileOut.close();
    this->setChanged(false);
}

/* load from file */
void Document::load(QString filename) {
    this->filename = filename;
    this->title = QFileInfo(filename).baseName() + ".sve";
    QFile fileIn(this->filename);
    fileIn.open(QFile::ReadOnly);
    QString file = fileIn.readAll();
    QString erm;
    int l, c;
    if (!this->xml->setContent(file, &erm, &l, &c)) {
        qDebug() << erm << ':' << l << '-' << c;
    }
    fileIn.close();

    this->setChanged(false);
    this->history.clear();
    this->pushToHistory();
}

/* set document changed flag */
void Document::setChanged(bool changed) {
    this->changed = changed;
}

/* handle signals from child elements */
void Document::handleChildSignals(AlterType type) {
    if (type != AlterType::None) {
        this->pushToHistory();
        this->setChanged(true);
        emit altered(true);
    }
    // redraw links if element was moved
    if ((type == AlterType::Moved) && (this->sender()->objectName() == "element_node")) {
        foreach(QObject *item, this->workarea->children()) {
            if ((item->objectName() == "link_node") && ((LinkNode*)item)->hasNode(((UNode*)this->sender())->getID())) {
                ((UNode*)item)->repaint();
            }
        }
    }
    // delete links if element was deleted
    if ((type == AlterType::Deleted) && (this->sender()->objectName() == "element_node")) {
        foreach(QObject *item, this->workarea->children()) {
            if ((item->objectName() == "link_node") && ((LinkNode*)item)->hasNode(((UNode*)this->sender())->getID())) {
                ((UNode*)item)->remove();
            }
        }
    }
}

/* set last clicked element for all kinds of stuff */
void Document::setActiveElement() {
    if (this->mode == DocumentMode::SelectNode && this->nodeCounter < 2) {
        this->nodeCounter++;
        this->activeElement = (UNode*)this->sender();
        emit(elementActivated(this->activeElement, this->nodeCounter));
    }
}

/* reset active element */
void Document::resetActiveElement() {
    this->activeElement = 0;
}

/* save xml tree to history */
void Document::pushToHistory() {
    QByteArray state = this->xml->toByteArray();
    this->history.push(state);
}

/* get screenshot of workarea */
QPixmap Document::getImage() {
    QPixmap img(this->workarea->size());
    this->workarea->render(&img);
    return img;
}

QString Document::getVHDL() {
    return
            "    -- (this is a VHDL comment)"
            "    "
            "    -- import std_logic from the IEEE library"
            "    library IEEE;"
            "    use IEEE.std_logic_1164.all;"
            "    "
            "    -- this is the entity"
            "    entity ANDGATE is"
            "      port ("
            "        I1 : in std_logic;"
            "        I2 : in std_logic;"
            "        O  : out std_logic);"
            "    end entity ANDGATE;"
            "    "
            "    -- this is the architecture"
            "    architecture RTL of ANDGATE is"
            "    begin"
            "      O <= I1 and I2;"
            "    end architecture RTL;";
}

/* return document's xml */
QDomDocument *Document::getXml() {
    return this->xml;
}

/* create widgets from xml tree */
void Document::renderNodes() {
    // remove children
    QObjectList children = this->workarea->children();
    foreach(QObject* child, children) {
        delete child;
    }
    // append children
    QDomNodeList labels = this->xml->elementsByTagName("label");
    int l = labels.size();
    for (int i = 0; i < l; i++) {
        this->addLabel(labels.at(i), true);
    }
    QDomNodeList nodes = this->xml->elementsByTagName("node");
    l = nodes.size();
    for (int i = 0; i < l; i++) {
        this->addNode(nodes.at(i), true);
    }
    QDomNodeList links = this->xml->elementsByTagName("link");
    l = links.size();
    for (int i = 0; i < l; i++) {
        this->addLink(links.at(i), true);
    }
}

/* get plugin by its name */
Plugin* Document::getPlugin(QString name) {
    foreach(Plugin *p, this->plugins) {
        if (p->getName() == name) {
            return p;
        }
    }
    return 0;
}

/* get list of plugins */
QStringList Document::getPlugins() {
    QStringList ps;
    foreach(Plugin *p, this->plugins) {
        ps << p->getName();
    }
    return ps;
}

/* set plugins */
void Document::setPlugins(QList<Plugin *> plugins) {
    this->plugins = plugins;
}

/* set document mode */
void Document::setMode(DocumentMode documentMode) {
    this->mode = documentMode;
    switch(documentMode) {
    case DocumentMode::Default:
        this->nodeCounter = 0;
        break;
    default:
        break;
    }
}

/* set node counter */
void Document::setNodeCounter(uint counter) {
    this->nodeCounter = counter;
}
