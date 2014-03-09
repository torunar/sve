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

class UNode : public QLabel
{
    Q_OBJECT
public:
    UNode(              QWidget *parent = 0);
    UNode(QString text, QWidget *parent = 0);
    UNode(const UNode &unode);
    ~UNode(){}

    QDomElement  node;
    QDomDocument *xml;

    void setPosition(int x, int y);

protected:
    QPoint startPos;

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *);
    void performDrag(const QPoint endPos);
    
signals:
    void altered(int signalType);
    /*
      0: None
      1: Moved
      2: Text edited
      3: Deleted
    */
    void activated(QDomElement);
    
public slots:

private slots:
    void remove();
    
};

#endif // UNODE_H
