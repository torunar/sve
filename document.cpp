#include "document.h"

/* @parent : MDI subwindow */
Document::Document(QMdiSubWindow *parent = 0) {
    this->parent = parent;

    /* scroll area inside of subwindow */
    this->container = new QScrollArea(this->parent);

    /* finally, workarea */
    this->workarea = new QFrame(this->container);
    QPalette pal = this->workarea->palette();
    pal.setColor(QPalette::Window, QColor(255,255,255));
    this->workarea->setPalette(pal);
    this->workarea->setAutoFillBackground(true);

    this->container->setWidget(workarea);

    if (parent) this->parent->setWidget(this->container);

    this->changed = false;
}

void Document::resize(const int w, const int h) {
    this->workarea->resize(w, h);
}

void Document::resize(const QSize size) {
    this->workarea->resize(size);
}

void Document::attachToWindow(QMdiSubWindow *parent) {
    this->parent = parent;
    this->container->setParent(this->parent);
    this->parent->setWidget(this->container);
}

bool Document::isChanged() {
    return this->changed;
}
