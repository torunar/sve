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
    Document *getDocument();

private:
    Document *document;
    void renderNodes();

protected:
    void closeEvent(QCloseEvent *closeEvent);

public slots:
    void setChanged(bool changed);
    void addLabel();
    void addElementNode();
    void save();
    bool load();
};

#endif // DOCWINDOW_H
