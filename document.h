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

#include "labelnode.h"
#include "elementnode.h"
#include "linknode.h"

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

    void addLink(QList<UNode *> elementNodes, QPair<int, int> connectors);
    void addLink(const QDomNode node);

    void renderNodes();

    Plugin     *getPlugin(QString name);
    QStringList getPlugins();
    void        setPlugins(QList<Plugin*> plugins);

    void setMode(DocumentMode documentMode);
    void setNodeCounter(uint counter);

    void resetActiveElement();

    QDomDocument *getXml();
    QString       title;
    QString       filename;

    uint inCounter;
    uint outCounter;

signals:
    void altered(bool);
    void elementActivated(UNode*, uint);

public slots:
    void handleChildSignals(AlterType type);
    void setActiveElement();
    void save(QString filename);
    void load(QString filename);
    void addNode(QString plugin);

private:
    QMdiSubWindow *parent;
    QScrollArea   *container;
    QFrame        *workarea;
    QDomDocument  *xml;

    bool         changed;
    DocumentMode mode;

    QList<Plugin*> plugins;

    UNode *activeElement;
    uint   nodeCounter;
};

#endif // DOCUMENT_H
