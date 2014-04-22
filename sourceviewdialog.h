#ifndef SOURCEVIEWDIALOG_H
#define SOURCEVIEWDIALOG_H

#include <QDialog>
#include <QWebView>
#include <QFile>
#include <QDebug>

namespace Ui {
class SourceViewDialog;
}

class SourceViewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SourceViewDialog(QWidget *parent = 0);
    ~SourceViewDialog();

    void setSource(QString src);
private:
    // TODO: replace with js and css content here
    QString htmlTemplate = "<html><head><style><!--#CSS#--></style><script type=\"text/javascript\"><!--#JS#--></script></head><body><div contenteditable=\"true\" id=\"code\"><!--#VHDL#--></div><script type=\"text/javascript\"><!--#MAIN#--></script></body></html>";
    QWebView *view;
    Ui::SourceViewDialog *ui;
};

#endif // SOURCEVIEWDIALOG_H
