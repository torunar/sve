#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWebKit/QWebView>
#include "docwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    Document *activeDocument;
    DocWindow *activeWindow;

    ~MainWindow();
    QSettings *settings;
    
private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;

public slots:
    void createDocument();
    void setActiveDocument();
    void quit();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
