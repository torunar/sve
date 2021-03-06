#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QListWidgetItem>
#include <QDialog>

#include "elementnode.h"

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ConnectionDialog(QList<UNode *> nodes, QWidget *parent = 0);
    ~ConnectionDialog();

    uint inCounter, outCounter;
    void setCounters(uint inCounter, uint outCounter);

    // indecies of output and input
    int selectedInput, selectedOutput;
    QPair<int, int> getConnectors();
    void            setConnectors(QPair<int, int> connectors);

signals:
    void selectedConnectors(int output, int input);

private:
    Ui::ConnectionDialog *ui;

public slots:
    void setInput(QListWidgetItem *);
    void setOutput(QListWidgetItem *);
};

#endif // CONNECTIONDIALOG_H
