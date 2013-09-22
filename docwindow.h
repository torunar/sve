#ifndef DOCWINDOW_H
#define DOCWINDOW_H

#include <QWidget>
#include <QMdiSubWindow>
#include <document.h>
#include <QString>
#include <QObject>

class DocWindow : public QMdiSubWindow {
    Q_OBJECT

public:
    DocWindow(QWidget *parent);
    ~DocWindow(){}

    void setTitle(const QString title);
    Document* getDocument();
    void setChanged();
    void setChanged(bool changed);

private:
    Document* document;

protected:
    void closeEvent(QCloseEvent *closeEvent);
    bool eventFilter(QObject *object, QEvent *event);
    void changeEvent(QEvent *changeEvent);

public slots:
    void addLabel();
    void save();

};

#endif // DOCWINDOW_H
