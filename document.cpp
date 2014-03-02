#include "document.h"

/*
 * @ QMdiSubWindow parent : parent to embed document into
 */
Document::Document(QMdiSubWindow *parent) {
    this->parent = parent;
    this->setObjectName("Doc");

    // scroll area inside of subwindow
    this->container = new QScrollArea(this->parent);

    // finally, workarea
    this->workarea = new QFrame(this->container);
    this->workarea->setObjectName("DocumentArea");
    this->workarea->setFrameStyle(QFrame::StyledPanel);

    this->container->setWidget(workarea);

    if (parent) this->parent->setWidget(this->container);

    this->title = "";

    this->changed = false;
    this->inCounter = 0;
    this->outCounter = 0;

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
    EditableLabel *label = new EditableLabel(text, this->xml, this->workarea);
    connect(label, SIGNAL(altered(int)), this, SLOT(handleChildSignals(int)));
    this->changed = true;
}
void Document::addLabel(const QDomNode node) {
    EditableLabel *label = new EditableLabel(node, this->xml, this->workarea);
    connect(label, SIGNAL(altered(int)), this, SLOT(handleChildSignals(int)));
    this->changed = true;
}

// add plugin node
void Document::addNode(Plugin *plugin) {
    ElementNode *elementNode = new ElementNode(plugin, this->xml, this->workarea);
    connect(elementNode, SIGNAL(altered(int)), this, SLOT(handleChildSignals(int)));
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
    connect(elementNode , SIGNAL(altered(int)), this, SLOT(handleChildSignals(int)));
    // update internal counters
    elementNode->setCounters(inCounter, outCounter);
    this->inCounter  += plugin->getInputs().size();
    this->outCounter += plugin->getOutputs().size();
    // set changed flag
    this->changed = true;
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

/*
 * Set document changed flag
 */
void Document::setChanged(bool changed) {
    this->changed = changed;
}

/*
 * Handle signals from child elements like labels and nodes
 */
void Document::handleChildSignals(int signalType) {
    if (signalType != 0) {
        setChanged(true);
        emit altered(true);
    }
}

QDomDocument *Document::getXml() {
    return this->xml;
}

void Document::renderNodes() {
    qDebug() << this->xml->toString();
    QDomNodeList labels = this->xml->elementsByTagName("label");
    QDomNodeList nodes = this->xml->elementsByTagName("node");
    // so we can't iterate with foreach, huh?
    for (int i = 0; i < labels.size(); i++) {
        this->addLabel(labels.at(i));
    }
    for (int i = 0; i < nodes.size(); i++) {
        this->addNode(nodes.at(i));
    }
    qDebug() << this->xml->toString();
}

Plugin* Document::getPlugin(QString name) {
    foreach(Plugin *p, this->plugins) {
        if (p->getName() == name) {
            return p;
        }
    }
    return 0;
}

void Document::setPlugins(QList<Plugin *> plugins) {
    this->plugins = plugins;
}
