#include "docwindow.h"


DocWindow::DocWindow(QWidget *parent) : QMdiSubWindow(parent) {
    int n = ((QMdiArea*) parent)->subWindowList().count() + 1;
    this->setWindowTitle(QObject::tr("Untitled") + " " + QString::number(n));
    this->showMaximized();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setObjectName("document_window");

    document = new Document(this);
    // to set window title
    connect(document, SIGNAL(altered(bool)), this, SLOT(setChanged(bool)));
}

void DocWindow::setTitle(const QString title) {
    this->setWindowTitle(title);
}

Document* DocWindow::getDocument() {
    return this->document;
}

void DocWindow::closeEvent(QCloseEvent *closeEvent) {
    if (this->document->isChanged()) {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle(tr("Document was changed"));
        mb->setText(tr("Document \"%1\" has some unsaved changes").arg(this->windowTitle()));
        mb->addButton(tr("Close without saving"), QMessageBox::DestructiveRole);
        mb->addButton(tr("Save"), QMessageBox::AcceptRole);
        mb->addButton(tr("Cancel"), QMessageBox::RejectRole);
        switch (mb->exec()) {
        // close without saving
        case 0:
            closeEvent->accept();
            break;
        // save
        case 1:
            this->save();
            closeEvent->accept();
            break;
        // cancel
        case 2:
            closeEvent->ignore();
            break;
        }
    }
    else {
        closeEvent->accept();
    }
}

void DocWindow::save() {
    QString filename;
    if (this->document->filename == "") {
        QFileDialog *fd = new QFileDialog();
        fd->setDefaultSuffix(".sve");
        filename = fd->getSaveFileName(0, tr("Save as..."), "", "*.sve");
        // saving cancelled
        if (filename == "") return;
        // setDefaultSuffix isn't working properly under Linux
        if (!filename.endsWith(".sve")) filename += ".sve";
    }
    else {
        filename = this->document->filename;
    }
    if (this->document->save(filename)) {
        this->setTitle(this->document->title);
        this->setStatus(tr("Document saved"), 2000);
    }
    else {
        this->setStatus(tr("Error saving document"), -1);
    }
}

void DocWindow::saveAs() {
    QString backupFilename = this->document->filename;
    this->document->filename = "";
    this->save();
    if (this->document->filename == "") {
        this->document->filename = backupFilename;
    }
}

bool DocWindow::load() {
    QFileDialog *fd = new QFileDialog();
    fd->setDefaultSuffix(".sve");
    QString filename = fd->getOpenFileName(0, tr("Open file..."), "", "*.sve");
    // saving cancelled
    if (filename == "") {
        return false;
    }
    // loading non-sve
    if (this->document->load(filename)) {
        this->setTitle(this->document->title);
        if (this->renderNodes()) {
            this->document->setChanged(false);
            this->setStatus(tr("Document loaded"), 2000);
            return true;
        }
        // no required plugin
        else {
            this->setStatus(tr("Document loading error: no required plugin"), -1);
        }
    }
    else {
        this->setStatus(tr("Document loading error: wrong format"), -1);
    }
    return false;

}

void DocWindow::setLinkNode(UNode *node, uint nodeCounter) {
    ConnectionDialog *cd;
    this->linkNodes << node;
    switch(nodeCounter) {
    case 1:
        this->setStatus(tr("Set ending node"), 0);
        break;
    case 2:
        // clear
        this->document->setMode(DocumentMode::Default);
        this->setStatus("", 0);
        // show slot selector
        cd = new ConnectionDialog(this->linkNodes);
        if (cd->exec() == QDialog::Accepted) {
            // output, input
            QPair<int, int> connectors = cd->getConnectors();
            this->document->addLink(this->linkNodes, connectors);
        }
        this->linkNodes.clear();
        delete cd;
        break;
    }
}

void DocWindow::showOptionsDialog() {
    DocumentOptionsDialog *dod = new DocumentOptionsDialog();
    dod->setDimensions(this->document->getSize());
    if (dod->exec() == QDialog::Accepted) {
        this->document->resize(dod->getDimensions());
        this->setChanged(true);
    }
    delete dod;
}

void DocWindow::showSaveImageDialog() {
    QFileDialog *fd = new QFileDialog();
    fd->setDefaultSuffix(".png");
    QString filename = fd->getSaveFileName(0, tr("Save as..."), "", "*.png");
    // saving cancelled
    if (filename == "") return;
    // setDefaultSuffix isn't working properly under Linux
    if (!filename.endsWith(".sve")) filename += ".png";
    QPixmap img = this->document->getImage();
    img.save(filename, 0, -1);
}

void DocWindow::viewVHDL() {
    QString vhdl = this->document->getVHDL();
    SourceViewDialog *svd = new SourceViewDialog(vhdl);
    svd->setSource(vhdl.replace("\n", "<br>\n"));
    svd->exec();
}

void DocWindow::addLabel() {
    bool ok;
    QString text = QInputDialog::getText(0,
        tr("New label"),
        tr("Insert text for this label"),
        QLineEdit::Normal,
        "",
        &ok
    );
    if (ok && !text.isEmpty()) {
        this->document->addLabel(text);
        this->setChanged(true);
    }
}

void DocWindow::addNode() {
    AddNodeDialog *ad = new AddNodeDialog(this->document->getPlugins());
    connect(ad, SIGNAL(itemSelected(QString)), this->document, SLOT(addNode(QString)));
    ad->exec();
    this->setChanged(this->document->isChanged());
}

void DocWindow::addLink() {
    this->setStatus(tr("Set beginning node"), 0);
    this->document->setMode(DocumentMode::SelectNode);
    // futher opertations take place in setLinkNode
    connect(this->document, SIGNAL(elementActivated(UNode*, uint)), this, SLOT(setLinkNode(UNode*, uint)), Qt::UniqueConnection);
}

void DocWindow::addNode(Plugin *plugin) {
    this->document->addNode(plugin);
    this->setChanged(true);
}

void DocWindow::attachStatusBar(QStatusBar *statusBar) {
    this->statusBar = statusBar;
}

void DocWindow::setChanged(bool changed) {
    this->document->setChanged(changed);
    if (changed && !this->windowTitle().endsWith("*")) {
        this->setWindowTitle(this->windowTitle() + "*");
    }
}

bool DocWindow::renderNodes() {
    return this->document->renderNodes();
}

void DocWindow::setStatus(QString text, int timeout = 0) {
    this->statusBar->showMessage(text, timeout);
}
