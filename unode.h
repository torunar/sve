#ifndef UNODE_H
#define UNODE_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
// context menu
#include <QMenu>
// xml
#include <QtXml/QDomElement>
// id in xml
#include <QDateTime>

#include <QDebug>

typedef enum {
    None    = 0,
    Moved   = 1,
    Edited  = 2,
    Deleted = 3
} AlterType;

class UNode : public QLabel
{
    Q_OBJECT
public:
    UNode(              QWidget *parent = 0);
    UNode(QString text, QWidget *parent = 0);
    UNode(const UNode &unode);
    ~UNode(){}

    QDomElement   node;
    QDomDocument *xml;
    QString getID();

    void setPosition(int x, int y);

protected:
    QPoint startPos;

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *);
    void performDrag(const QPoint endPos);
    
signals:
    void altered(AlterType);
    void activated();
    
public slots:

private slots:
    void remove();
    
};

#endif // UNODE_H
