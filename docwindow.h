#ifndef DOCWINDOW_H
#define DOCWINDOW_H

#include <QWidget>
#include <QMdiSubWindow>
#include "document.h"
#include <QString>
#include <QObject>
#include <QStatusBar>

#include "addnodedialog.h"
#include "connectiondialog.h"
#include "documentoptionsdialog.h"

class DocWindow : public QMdiSubWindow {
    Q_OBJECT

public:
    DocWindow(QWidget *parent);
    ~DocWindow(){}

    void      setTitle(const QString title);
    Document *getDocument();

    void addNode(Plugin *plugin);
    void attachStatusBar(QStatusBar *statusBar);

private:
    Document      *document;
    QStatusBar    *statusBar;
    QList<UNode *> linkNodes;

    void renderNodes();
    void setStatus(QString text, int timeout);

protected:
    void closeEvent(QCloseEvent *closeEvent);

public slots:
    void setChanged(bool changed);
    void addLabel();
    void addNode();
    void addLink();
    void save();
    void saveAs();
    bool load();
    void setLinkNode(UNode *node, uint nodeCounter);
    void showOptionsDialog();
    void showSaveImageDialog();
};

#endif // DOCWINDOW_H
