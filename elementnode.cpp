#include "elementnode.h"

ElementNode::ElementNode(QWidget *parent) : EditableLabel(parent){
    QPixmap background("../plugins/and/plugin.svg");
    background = background.scaled(80, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(background);
    this->show();
}
