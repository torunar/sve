#ifndef LINKWIDGET_H
#define LINKWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include "nodelink.h"

class LinkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LinkWidget(QWidget *parent = 0);

private:
    QPainter *painter;
    QImage   buffer;
    QPen     pen;
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:
    
};

#endif // SCROLLAREAPAINTER_H
