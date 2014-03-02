#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class AddNodeDialog;
}

class AddNodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddNodeDialog(QStringList plugins, QWidget *parent = 0);
    ~AddNodeDialog();

signals:
    void itemSelected(QString item);

private:
    Ui::AddNodeDialog *ui;

public slots:
    void getSelection();
    void unlockAdd();
};

#endif // ADDNODEDIALOG_H
