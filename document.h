#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QWidget>
#include <QScrollArea>
#include <QMdiArea>
#include <QMdiSubWindow>

class Document
{
public:
    Document(QMdiSubWindow *parent);

    void attachToWindow(QMdiSubWindow *parent);
    void resize(const QSize size);
    void resize(const int w, const int h);
    bool isChanged();

private:
    QMdiSubWindow* parent;
    QScrollArea* container;
    QFrame* workarea;
    bool changed;

};

#endif // DOCUMENT_H
