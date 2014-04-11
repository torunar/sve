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
    this->document->save(filename);
    this->setTitle(this->document->title);
    this->setStatus(tr("Document saved"), 2000);
}

bool DocWindow::load() {
    QFileDialog *fd = new QFileDialog();
    fd->setDefaultSuffix(".sve");
    QString filename = fd->getOpenFileName(0, tr("Open file..."), "", "*.sve");
    // saving cancelled
    if (filename == "") return false;
    this->document->load(filename);
    this->setTitle(this->document->title);
    this->renderNodes();
    this->document->setChanged(false);
    this->setStatus(tr("Document loaded"), 2000);
    return true;
}

void DocWindow::setLinkNode(UNode *node, uint nodeCounter) {
    qDebug() << node->node.attribute("id") << nodeCounter;
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
        cd->setCounters(this->document->inCounter, this->document->outCounter);
        if (cd->exec() == QDialog::Accepted) {
            this->document->addLink(this->linkNodes);
        }
        this->linkNodes.clear();
        delete cd;
        break;
    }
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
    // future opertations take place in setLinkNode
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

void DocWindow::renderNodes() {
    this->document->renderNodes();
}

void DocWindow::setStatus(QString text, int timeout = 0) {
    this->statusBar->showMessage(text, timeout);
}
