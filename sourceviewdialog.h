#ifndef SOURCEVIEWDIALOG_H
#define SOURCEVIEWDIALOG_H

#include <QDialog>
#include <QWebView>

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
    QString htmlTemplate = "<html><head><link rel=\"stylesheet\" href=\"hl.css\"><script src=\"hl.js\"></script></head><body><pre contenteditable=\"true\" id=\"code\"><!--#VHDL#--></pre><script>var h = document.getElementById('code');hljs.highlightBlock(h);</script></body></html>";
    QWebView *view;
    Ui::SourceViewDialog *ui;
};

#endif // SOURCEVIEWDIALOG_H
