#ifndef NODEPROPERTIESWINDOW_H
#define NODEPROPERTIESWINDOW_H

#include <QDialog>
#include <QAbstractButton>
#include <QDebug>

namespace Ui {
class NodePropertiesWindow;
}

class NodePropertiesWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodePropertiesWindow(QWidget *parent = 0);
    ~NodePropertiesWindow();
    void setSource(QString source);
    void setInputs(QVector<QString> *inputs);
    void setOutputs(QVector<QString> *outputs);
    
private:
    Ui::NodePropertiesWindow *ui;

public slots:
    void save();
};

#endif // NODEPROPERTIESWINDOW_H
