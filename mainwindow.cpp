#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings = new QSettings("torunar", "sve");
    settings->beginGroup("DefaultDoc");

    ui->setupUi(this);

    this->setStyleSheet(
                "#DocumentArea {"
                    "background: #fff;"
                    "background-image: url(:/ui/grid);"
                    "background-repeat: repeat-xy;"
                "}"
                "#EditableLabel:hover {"
                    "background: #eee;"
                "}"
    );

    for (int i = 0; i < 5; i++) {
        ui->pluginBar->addAction(QIcon(":/icons/plugin-empty"), "NULL");
    }

    /* set MDI options */
    this->mdiArea = new QMdiArea;
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setViewMode(QMdiArea::TabbedView);
    this->setCentralWidget(mdiArea);

    this->activeDocument = NULL;

    this->createDocument();

    connect(ui->aNew, SIGNAL(triggered()), this, SLOT(createDocument()));
    connect(ui->aQuit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(this->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(setActiveDocument(QMdiSubWindow*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    exit(0);
}

void MainWindow::createDocument() {
    if (this->activeDocument) {
        disconnect(ui->aAddNode, SIGNAL(triggered()), this->activeDocument, SLOT(addNode()));
    }
    DocWindow *docwindow = new DocWindow(mdiArea);
    this->activeDocument = docwindow->getDocument();
    connect(ui->aAddNode, SIGNAL(triggered()), this->activeDocument, SLOT(addNode()));
    this->activeDocument->resize(settings->value("Size").toSize());
}

void MainWindow::setActiveDocument(QMdiSubWindow *docwindow) {
    // something?
}

