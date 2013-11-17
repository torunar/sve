#include "docwindow.h"


DocWindow::DocWindow(QWidget *parent) : QMdiSubWindow(parent) {
    int n = ((QMdiArea*) parent)->subWindowList().count() + 1;
    this->setWindowTitle(QObject::tr("Untitled") + " " + QString::number(n));
    this->showMaximized();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setObjectName("DocWindow");

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
        // "delete this" is a trick to really DeleteOnClose as defined above
        switch (mb->exec()) {
        // close without saving
        case 0:
            closeEvent->accept();
            delete this;
            break;
        // save
        case 1:
            this->save();
            closeEvent->accept();
            delete this;
            break;
        // cancel
        case 2:
            closeEvent->ignore();
            break;
        }
    }
    else {
        closeEvent->accept();
        delete this;
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
    return true;
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

void DocWindow::addElementNode() {
    this->document->addElementNode();
    this->setChanged(true);
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
