#ifndef NODELINK_H
#define NODELINK_H

#include <QImage>
#include <QPainter>
#include "editablelabel.h"
#include "nodelink.h"

class NodeLink : public EditableLabel
{
    Q_OBJECT
public:
    explicit NodeLink(QWidget *parent = 0);

private:
    QPainter *painter;
    QImage   buffer;
    QPen     pen;
    void paintEvent(QPaintEvent *);
    QVector<QPoint> line;

signals:

public slots:

};

#endif // NODELINK_H
