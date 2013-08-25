#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QWidget>
#include <QScrollArea>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QtXml/QDomDocument>
#include <QLabel>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <editablelabel.h>

typedef enum {
    Stub = 0,
    Entity = 1,
    Label = 2
} NodeType;

class Document: public QObject
{
    Q_OBJECT

public:
    Document(QMdiSubWindow *parent = 0);
    ~Document(){}

    void attachToWindow(QMdiSubWindow *parent);
    void resize(const QSize size);
    void resize(const int w, const int h);
    bool isChanged();

    QString title;

public slots:
    void addNode(QString nodeName = "", const NodeType nodeType = Label);
//    void addLink(const QString linkName){}

private:
    QMdiSubWindow *parent;
    QScrollArea *container;
    QFrame *workarea;
    QDomDocument *content;
    bool changed;

};

#endif // DOCUMENT_H
