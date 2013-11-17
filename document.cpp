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

void Document::addElementNode() {
    ElementNode *elementNode = new ElementNode(this->workarea);
    connect(elementNode, SIGNAL(altered(int)), this, SLOT(handleChildSignals(int)));
    this->changed = true;
}

/*
 * Add any abstract node
 * TODO: probably, remove
 */
void Document::addNode(const QString nodeName = NULL, const NodeType nodeType = Stub) {
    qDebug() << nodeName << nodeType;
}

/*
 * Save document to file
 */
void Document::save(QString filename) {
    this->filename = filename;
    this->title = QFileInfo(filename).baseName() + ".sve";
    QFile fileOut(this->filename);
    fileOut.open(QFile::WriteOnly);
    fileOut.write(this->xml->toString().toAscii());
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
    int n = labels.size();
    for (int i = 0; i < n; i++) {
        this->addLabel(labels.at(i));
    }
    qDebug() << this->xml->toString();
}
