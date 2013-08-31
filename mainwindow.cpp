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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    exit(0);
}

void MainWindow::createDocument() {
    // disconnect old slots
    if (this->activeDocument) {
        disconnect(ui->aAddNode, SIGNAL(triggered()), this->activeDocument, SLOT(addNode()));
    }
    // create new window
    DocWindow *docwindow = new DocWindow(mdiArea);
    connect(docwindow, SIGNAL(aboutToActivate()), this, SLOT(setActiveDocument()));
    // set context
    this->activeDocument = docwindow->getDocument();
    connect(ui->aAddNode, SIGNAL(triggered()), this->activeDocument, SLOT(addNode()));
    // resize
    this->activeDocument->resize(settings->value("Size").toSize());
}

void MainWindow::setActiveDocument() {
    // disconnect old slots
    if (this->activeDocument) {
        disconnect(ui->aAddNode, SIGNAL(triggered()), this->activeDocument, SLOT(addNode()));
    }
    // switch context
    this->activeDocument = ((DocWindow*) this->sender())->getDocument();
    // connect new slots
    connect(ui->aAddNode, SIGNAL(triggered()), this->activeDocument, SLOT(addNode()));
}

