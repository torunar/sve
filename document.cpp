#include "document.h"

/*
 * @ QMdiSubWindow parent : parent to embed document into
 */
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
}

/*
 * @ int w: width
 * @ int h: height
 */
void Document::resize(const int w, const int h) {
    this->workarea->resize(w, h);
}

/*
 * @ QSize size: new size to resize to
 */
void Document::resize(const QSize size) {
    this->workarea->resize(size);
    this->xml->firstChild().toElement().setAttribute("width",  size.width());
    this->xml->firstChild().toElement().setAttribute("height", size.height());
}

/*
 * @ QMdiSubWindow parent: parent to attach document to
 */
void Document::attachToWindow(QMdiSubWindow *parent) {
    this->parent = parent;
    this->container->setParent(this->parent);
    this->parent->setWidget(this->container);
}

/*
 * @return: check if document tree was changed
 */
bool Document::isChanged() {
    return this->changed;
}

/*
 * Add label to the document and xml tree
 */
void Document::addLabel(const QString text) {
    LabelNode *label = new LabelNode(text, this->xml, this->workarea);
    connect(label, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    this->changed = true;
}
void Document::addLabel(const QDomNode node) {
    LabelNode *label = new LabelNode(node, this->xml, this->workarea);
    connect(label, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    this->changed = true;
}

// add plugin node
void Document::addNode(Plugin *plugin) {
    ElementNode *elementNode = new ElementNode(plugin, this->xml, this->workarea);
    connect(elementNode, SIGNAL(activated()),        this, SLOT(setActiveElement()));
    connect(elementNode, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
    // update internal counters
    elementNode->setCounters(inCounter, outCounter);
    this->inCounter  += plugin->getInputs().size();
    this->outCounter += plugin->getOutputs().size();
    // set changed flag
    this->changed = true;
}
void Document::addNode(const QDomNode node){
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
    this->changed = true;
}

void Document::addLink(QList<UNode*> elementNodes) {
    LinkNode *nl = new LinkNode(elementNodes, this->xml, this->workarea);
    this->setMode(DocumentMode::Default);
    this->changed = true;
}

void Document::addLink(const QDomNode node) {
}

/*
 * Save document to file
 */
void Document::save(QString filename) {
    this->filename = filename;
    this->title = QFileInfo(filename).baseName() + ".sve";
    QFile fileOut(this->filename);
    fileOut.open(QFile::WriteOnly);
    fileOut.write(this->xml->toString().toUtf8());//.toAscii());
    fileOut.close();
    this->setChanged(false);
}

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
    this->setChanged(false);
    fileIn.close();
}

void Document::addNode(QString plugin){
    this->addNode(this->getPlugin(plugin));
}

/*
 * Set document changed flag
 */
void Document::setChanged(bool changed) {
    this->changed = changed;
}

/*
 * Handle signals from child elements like labels and nodes
 */
void Document::handleChildSignals(AlterType type) {
    if (type != AlterType::None) {
        setChanged(true);
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
}

/*
 * Last clicked element for stuff
 */
void Document::setActiveElement() {
    if (this->mode == DocumentMode::SelectNode && this->nodeCounter < 2) {
        this->nodeCounter++;
        this->activeElement = (UNode*)this->sender();
        qDebug() << "emit";
        emit(elementActivated(this->activeElement, this->nodeCounter));
    }
}

void Document::resetActiveElement() {
    this->activeElement = 0;
}

QDomDocument *Document::getXml() {
    return this->xml;
}

void Document::renderNodes() {
    QDomNodeList labels = this->xml->elementsByTagName("label");
    QDomNodeList nodes = this->xml->elementsByTagName("node");
    // so we can't iterate with foreach, huh?
    for (int i = 0; i < labels.size(); i++) {
        this->addLabel(labels.at(i));
    }
    for (int i = 0; i < nodes.size(); i++) {
        this->addNode(nodes.at(i));
    }
}

Plugin* Document::getPlugin(QString name) {
    foreach(Plugin *p, this->plugins) {
        if (p->getName() == name) {
            return p;
        }
    }
    return 0;
}

QStringList Document::getPlugins() {
    QStringList ps;
    foreach(Plugin *p, this->plugins) {
        ps << p->getName();
    }
    return ps;
}

void Document::setPlugins(QList<Plugin *> plugins) {
    this->plugins = plugins;
}

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

void Document::setNodeCounter(uint counter) {
    this->nodeCounter = counter;
}
