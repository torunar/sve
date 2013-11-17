#ifndef ELEMENTNODE_H
#define ELEMENTNODE_H

#include <QPixmap>
#include <editablelabel.h>

class ElementNode : public EditableLabel
{
    Q_OBJECT
public:
    ElementNode(QWidget *parent = 0);
};

#endif // ELEMENTNODE_H
