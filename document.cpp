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
    // set changed flag
    this->setChanged(true);
}

/* add node by xml node */
bool Document::addNode(const QDomNode node, bool skipHistory){
    if (!skipHistory) {
        this->pushToHistory();
    }
    QString pluginName = node.toElement().attribute("plugin");
    if (this->getPlugins().contains(pluginName)) {
        Plugin *plugin = this->getPlugin(pluginName);
        ElementNode *elementNode = new ElementNode(node, plugin, this->xml, this->workarea);
        connect(elementNode, SIGNAL(activated()),        this, SLOT(setActiveElement()));
        connect(elementNode, SIGNAL(altered(AlterType)), this, SLOT(handleChildSignals(AlterType)));
        // set changed flag
        this->setChanged(true);
        return true;
    }
    else {
        this->setChanged(false);
        return false;
    }
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
bool Document::save(QString filename) {

    this->filename = filename;
    this->title = QFileInfo(filename).baseName() + ".sve";
    QFile fileOut(this->filename);
    fileOut.open(QFile::WriteOnly);
    bool res = (fileOut.write(this->xml->toString().toUtf8()) > 0);
    fileOut.close();
    if (res) {
        this->setChanged(false);
    }
    return res;
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

/* TODO: very raw extremely not optimized code */
/* This would be pretty nice to rewrite it     */
QString Document::getVHDL() {
    QString vhdl = "-- SVE direct export\n\n";
    QStringList usedPlugins = this->getUsedPlugins();

    QString portTemplate  = "\t%1 : %2 BIT";
    // %1 - name
    // %2 - type (in/out)

    QString entityTemplate = "entity %1 is\n"
    "port (\n"
    "%2"
    "%3"
    ");\n"
    "end entity %1;\n\n";
    // %1 - name
    // %2 - inputs
    // %3 - outputs

    QString functionalTemplate = "architecture functional of %1 is\n"
    "begin\n"
    "%2\n"
    "end architecture functional;\n\n";
    // %1 - name
    // %2 - source

    QString componentTemplate = "component %1 is\n"
    "port (\n"
    "%2"
    "%3"
    ");\n"
    "end component %1;\n\n";
    // %1 - name
    // %2 - inputs
    // %3 - outputs

    QString structureTemplate = "architecture structure of sve is\n"
    "%1"
    "%2\n"
    "begin\n"
    "%3"
    "end architecture structure;";
    // %1 - components
    // %2 - signals
    // %3 - source

    QStringList components;

    /* Enities of the system */
    foreach(QString name, usedPlugins) {
        if (name == "port_in" || name == "port_out") {
            continue;
        }
        Plugin *plugin = this->getPlugin(name);
        QVector<QString> pInputs = plugin->getInputs();
        QVector<QString> pOutputs = plugin->getOutputs();
        QString inputs, outputs, source;
        source = plugin->getSource();
        int summary = pInputs.size() + pOutputs.size();
        for (int i = 0; i < pInputs.size(); i++) {
            pInputs[i] = pInputs[i].replace("%INC%", QString::number(i + 1));
            inputs    += portTemplate.arg(pInputs[i], "in");
            if (i != summary) {
                inputs += ";\n";
            }
            source     = source.replace("%IN_" + QString::number(i + 1) + "%", pInputs[i]);
        }
        for (int i = 0; i < pOutputs.size(); i++) {
            pOutputs[i] = pOutputs[i].replace("%OUTC%", QString::number(i + 1));
            outputs    += portTemplate.arg(pOutputs[i], "out");
            if (i != pOutputs.size() - 1) {
                outputs += ";\n";
            }
            source      = source.replace("%OUT_" + QString::number(i + 1) + "%", pOutputs[i]);
        }
        // describe entities
        vhdl += entityTemplate.arg(name, inputs, outputs) + functionalTemplate.arg(name, source);
        // add components for system
        components << componentTemplate.arg(name, inputs, outputs);
    }

    /* Main entity */
    vhdl += "-- SVE entity\n";
    QMap<QString, QPair<QDomElement, int> > sInputs, sOutputs;
    QString inputs, outputs;
    QDomNodeList nodes = this->xml->elementsByTagName("node");
    int ns = nodes.size();
    int ic = 0;
    int oc = 0;
    for(int i = 0; i < ns; i++) {
        QDomElement element = nodes.at(i).toElement();
        QString usedPlugin = element.attribute("plugin");
        if (usedPlugin == "port_in") {
            sInputs[element.attribute("id")] = {element, ++ic};
        }
        if (usedPlugin == "port_out") {
            sOutputs[element.attribute("id")] = {element, ++oc};
        }
    }
    int summary = sInputs.size() + sOutputs.size();
    for(int i = 0; i < sInputs.size(); i++) {
        inputs += portTemplate.arg("SVEIN" + QString::number(i + 1), "in");
        if (i != summary) {
            inputs += ";\n";
        }
    }
    for(int i = 0; i < sOutputs.size(); i++) {
        outputs += portTemplate.arg("SVEOUT" + QString::number(i + 1), "out");
        if (i != sOutputs.size() - 1) {
            outputs += ";\n";
        }
    }
    vhdl += entityTemplate.arg("sve", inputs, outputs);

    // signals to inputs and outputs
    QDomNodeList elinks = this->xml->elementsByTagName("link");
    QMap<QString, QPair<QDomElement, int> > links;
    QStringList sSignals;
    QString source = "";
    for(int i = 0; i < elinks.size(); i++) {
        // put signal
        sSignals << "signal SVESIG" + QString::number(i + 1) + ": BIT;\n";
        QDomElement link = elinks.at(i).toElement();
        // append links
        links[link.attribute("id")] = {link, i + 1};
        QString fId = link.attribute("first_id");
        QString lId = link.attribute("last_id");
        // outer ports
        if (sInputs.contains(fId)) {
            source += QString("\tSVESIG%1 <= SVEIN%2;\n").arg(QString::number(i + 1), QString::number(sInputs[fId].second));
        }
        else if (sOutputs.contains(lId)) {
            source += QString("\tSVEOUT%2 <= SVESIG%1;\n").arg(QString::number(i + 1), QString::number(sOutputs[lId].second));
        }
    }
    // signals with same output
    for(int i = 0; i < elinks.size(); i++) {
        QDomElement link1 = elinks.at(i).toElement();
        QString fId1 = link1.attribute("first_id");
        QString cId1 = link1.attribute("first_connector");
        for(int j = i + 1; j < elinks.size(); j++) {
            QDomElement link2 = elinks.at(j).toElement();
            QString fId2 = link2.attribute("first_id");
            QString cId2 = link2.attribute("first_connector");
            if (fId1 == fId2 && cId1 == cId2) {
                source += QString("\tSVESIG%1 <= SVESIG%2;\n").arg(QString::number(j + 1), QString::number(i + 1));
            }
        }
    }
    // nodes
    for (int i = 0; i < ns; i++) {
        QDomElement node = nodes.at(i).toElement();
        QString usedPlugin = node.attribute("plugin");
        int maxInputs  = this->getPlugin(usedPlugin)->getInputs().size();
        int maxOutputs = this->getPlugin(usedPlugin)->getOutputs().size();
        if (usedPlugin != "port_in" && usedPlugin != "port_out") {
            QStringList inputs, outputs;
            for (int j = 0; j < links.values().size(); j++) {
                QPair<QDomElement, int> link = links.values().at(j);
                if (link.first.attribute("last_id") == node.attribute("id") && inputs.size() < maxInputs) {
                    // put signal accordingly to connector
                    inputs.insert(link.first.attribute("last_connector").toInt(), "SVESIG" + QString::number(link.second));
                }
                if (link.first.attribute("first_id") == node.attribute("id") && outputs.size() < maxOutputs) {
                    outputs.insert(link.first.attribute("first_connector").toInt(), "SVESIG" + QString::number(link.second));
                }
            }
            source += QString("\tSVENODE%1 : %2 port map (%3, %4);\n").arg(QString::number(i + 1), usedPlugin, inputs.join(", "), outputs.join(", "));
        }
    }

    vhdl += structureTemplate.arg(components.join(""), sSignals.join(""), source);

    return vhdl;
}

/* return document's xml */
QDomDocument *Document::getXml() {
    return this->xml;
}

/* create widgets from xml tree */
bool Document::renderNodes() {
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
        if (!this->addNode(nodes.at(i), true)) {
            return false;
        }
    }
    QDomNodeList links = this->xml->elementsByTagName("link");
    l = links.size();
    for (int i = 0; i < l; i++) {
        this->addLink(links.at(i), true);
    }
    return true;
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

QStringList Document::getUsedPlugins() {
    QDomNodeList nodes = this->xml->elementsByTagName("node");
    int l = nodes.size();
    QStringList usedPlugins;
    for (int i = 0; i < l; i++) {
        QString usedPlugin = nodes.at(i).toElement().attribute("plugin");
        if (!usedPlugins.contains(usedPlugin)) {
            usedPlugins << usedPlugin;
        }
    }
    return usedPlugins;
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
