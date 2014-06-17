#ifndef SOURCEVIEWDIALOG_H
#define SOURCEVIEWDIALOG_H

#include <QDialog>
#include <QWebView>
#include <QFile>
#include <QDebug>

#include <QProcess>
#include <QTemporaryFile>

namespace Ui {
class SourceViewDialog;
}

class SourceViewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SourceViewDialog(QString vhdlSource = "", QWidget *parent = 0);
    ~SourceViewDialog();

    void setSource(QString src);

private:
    QString htmlTemplate = "<html><head><style>%1</style><script type=\"text/javascript\">%2</script></head><body><div contenteditable=\"true\" id=\"code\">%3</div><script type=\"text/javascript\">%4</script></body></html>";
    QString vhdlSource;
    QWebView *view;
    Ui::SourceViewDialog *ui;
    bool ghdlNotFound = false;

public slots:
    void validate();
    void handleError(QProcess::ProcessError);
};

#endif // SOURCEVIEWDIALOG_H
