#ifndef DOCUMENTOPTIONSDIALOG_H
#define DOCUMENTOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class DocumentOptionsDialog;
}

class DocumentOptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DocumentOptionsDialog(QWidget *parent = 0);
    ~DocumentOptionsDialog();
    void setDimensions(QSize size);
    QSize getDimensions();
    
private:
    Ui::DocumentOptionsDialog *ui;
};

#endif // DOCUMENTOPTIONSDIALOG_H
