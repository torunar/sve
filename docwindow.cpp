#include "docwindow.h"


DocWindow::DocWindow(QMdiArea *parent = 0) {
    this->window = new QMdiSubWindow(parent);
    this->window->setWindowTitle(QObject::trUtf8("Untitled"));
    this->window->showMaximized();

    document = new Document(this->window);
}

void DocWindow::setTitle(const QString title) {
    this->window->setWindowTitle(title);
}

Document* DocWindow::getDocument() {
    return this->document;
}

QMdiSubWindow* DocWindow::getWindow() {
    return this->window;
}
