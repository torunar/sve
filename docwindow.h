#ifndef DOCWINDOW_H
#define DOCWINDOW_H

#include <QWidget>
#include <QMdiSubWindow>
#include <document.h>
#include <QString>
#include <QObject>

class DocWindow {
public:
    DocWindow(QMdiArea *parent);
    Document *document;
    QMdiSubWindow *window;

    void setTitle(const QString title);
    Document* getDocument();
    QMdiSubWindow* getWindow();
};

#endif // DOCWINDOW_H
