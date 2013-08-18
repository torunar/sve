#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QSettings settings("torunar", "sve");
    settings.beginGroup("DefaultDoc");

    ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        ui->pluginBar->addAction(QIcon(":default/icons/plugin-empty.png"), "NULL");
    }

    /* set MDI options */
    this->mdiArea = new QMdiArea;
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setViewMode(QMdiArea::TabbedView);
    this->setCentralWidget(mdiArea);

    Document* activeDocument;

    /*  create new subwindow */
    DocWindow* docwindow = new DocWindow(mdiArea);
    activeDocument = docwindow->getDocument();
    activeDocument->resize(settings.value("Size").toSize());

    settings.endGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}
