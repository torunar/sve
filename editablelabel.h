#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <QInputDialog>
#include <QMouseEvent>
#include <QDateTime>
#include <QtXml/QDomElement>
#include <QDebug>
#include <QEvent>
#include <QMdiSubWindow>

class EditableLabel : public QLabel
{
    Q_OBJECT
public:
    EditableLabel(const QString text, QDomDocument *xml, QWidget *parent = 0);
    ~EditableLabel(){}
    QDomElement node;
    QDomDocument* xml;

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    QPoint startPos;
    void performDrag(const QPoint endPos);
    
signals:

public slots:
    void showContextMenu(const QPoint &pos);

private slots:
    void editText();
    void deleteLabel();
};

#endif // EDITABLELABEL_H
