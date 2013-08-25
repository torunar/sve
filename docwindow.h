#ifndef DOCWINDOW_H
#define DOCWINDOW_H

#include <QWidget>
#include <QMdiSubWindow>
#include <document.h>
#include <QString>
#include <QObject>

class DocWindow : public QMdiSubWindow {
    Q_OBJECT

public:
    DocWindow(QWidget *parent);
    ~DocWindow(){}

    void setTitle(const QString title);
    Document* getDocument();

private:
    Document* document;
};

#endif // DOCWINDOW_H
