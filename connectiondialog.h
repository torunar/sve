#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QListWidgetItem>
#include <QDialog>
#include "elementnode.h"

#include <QDebug>

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

signals:
    void selectedConnectors(int output, int input);

private:
    Ui::ConnectionDialog *ui;

public slots:
    void setInput(QListWidgetItem *);
    void setOutput(QListWidgetItem *);
    void getSelection();
};

#endif // CONNECTIONDIALOG_H
