#include "docwindow.h"


DocWindow::DocWindow(QWidget *parent) : QMdiSubWindow(parent) {
    int n = ((QMdiArea*) parent)->subWindowList().count() + 1;
    this->setWindowTitle(QObject::tr("Untitled") + " " + QString::number(n));
    this->showMaximized();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setObjectName("DocWindow");

    document = new Document(this);
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

bool DocWindow::eventFilter(QObject *object, QEvent *event){
//    qDebug() << event;
    return false;
}

void DocWindow::changeEvent(QEvent *changeEvent) {

    if (changeEvent->type() == QEvent::WindowTitleChange) {
        this->setChanged();
    }
}

void DocWindow::setChanged(bool changed) {
    this->document->setChanged(changed);
    if (changed && !this->windowTitle().endsWith("*")) {
        this->setWindowTitle(this->windowTitle() + "*");
    }
}

void DocWindow::setChanged() {
    if (this->windowTitle().endsWith("*")) this->document->setChanged(true);
}


