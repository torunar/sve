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

#include "editablelabel.h"
#include "elementnode.h"
#include "nodelink.h"

typedef enum {
    Stub = 0,
    Entity = 1,
    Label = 2
} NodeType;

typedef enum {
    Default    = 0,
    SelectNode = 1
} DocumentMode;

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
    void addLabel(const QDomNode node);

    void addNode(Plugin *plugin);
    void addNode(const QDomNode node);

    void addLink();
    void addLink(const QDomNode node);

    void renderNodes();

    Plugin     *getPlugin(QString name);
    QStringList getPlugins();
    void        setPlugins(QList<Plugin*> plugins);

    void setMode(DocumentMode documentMode);

    QDomDocument *getXml();
    QString       title;
    QString       filename;

signals:
    void altered(bool);

public slots:
    void handleChildSignals(int signalType);
    void setActiveElement(QDomElement element);
    void save(QString filename);
    void load(QString filename);
    void addNode(QString plugin);

private:
    QMdiSubWindow *parent;
    QScrollArea *container;
    QList<Plugin*> plugins;
    QFrame *workarea;
    QDomDocument *xml;
    bool changed;

    uint inCounter;
    uint outCounter;
};

#endif // DOCUMENT_H
