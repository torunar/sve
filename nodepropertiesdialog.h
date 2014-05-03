#ifndef NODEPROPERTIESDIALOG_H
#define NODEPROPERTIESDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QDebug>

namespace Ui {
class NodePropertiesDialog;
}

class NodePropertiesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodePropertiesDialog(QWidget *parent = 0);
    ~NodePropertiesDialog();
    void setSource(QString source);
    void setInputs(QVector<QString> inputs);
    void setOutputs(QVector<QString> outputs);
    
private:
    Ui::NodePropertiesDialog *ui;
    QVector<QString> inputs;
    QVector<QString> outputs;

public slots:
    void save();
};

#endif // NODEPROPERTIESDIALOG_H
