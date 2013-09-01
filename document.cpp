#include "document.h"

/*
 * @ QMdiSubWindow parent : parent to embed document into
 */
Document::Document(QMdiSubWindow *parent) {
    this->parent = parent;

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

void Document::addNode(QString nodeName, const NodeType nodeType) {
    bool ok;
    if (nodeType == Label) {
        QString text = QInputDialog::getText(this->workarea,
            tr("New label"),
            tr("Insert text for this label"),
            QLineEdit::Normal,
            "",
            &ok
        );
        if (ok && !text.isEmpty()) {
            EditableLabel *label = new EditableLabel(text, this->xml, this->workarea);
            label->show();
        }
    }
    this->changed = true;
}

bool Document::save() {
    QString filename;
    if (this->title == "") {
        QFileDialog *fd = new QFileDialog();
        filename = fd->getSaveFileName(0, tr("Save as..."), "", "*.sve");
        this->title = QFileInfo(filename).baseName() + ".sve";
        if (!filename.contains(".sve")) filename += ".sve";
    }
    QFile fileOut(filename);
    fileOut.open(QFile::WriteOnly);
    fileOut.write(this->xml->toString().toAscii());
    fileOut.close();
    return true;
}
