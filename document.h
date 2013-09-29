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
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
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
    void setChanged(bool changed);

    void addLabel(const QString text);
    void addNode(const QString nodeName, const NodeType nodeType);
    QString title;
    QString filename;

signals:
    void altered(bool);

public slots:
    //void addLabel();
    //void addNode();
    void handleChildSignals(int signalType);
    void save(QString filename);

private:
    QMdiSubWindow *parent;
    QScrollArea *container;
    QFrame *workarea;
    QDomDocument *xml;
    bool changed;
};

#endif // DOCUMENT_H
