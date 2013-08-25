#include "docwindow.h"


DocWindow::DocWindow(QWidget *parent) : QMdiSubWindow(parent) {
    this->setWindowTitle(QObject::tr("Untitled"));
    this->showMaximized();

    document = new Document(this);
}

void DocWindow::setTitle(const QString title) {
    this->setWindowTitle(title);
}

Document* DocWindow::getDocument() {
    return this->document;
}
