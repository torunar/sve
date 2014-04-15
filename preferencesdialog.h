#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

public slots:
    void browsePath();
    void saveOptions();
    
private:
    Ui::PreferencesDialog *ui;
    QSettings *settings;
};

#endif // PREFERENCESDIALOG_H
