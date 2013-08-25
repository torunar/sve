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

    this->createDocument();

    this->setCentralWidget(mdiArea);

    connect(ui->aNew, SIGNAL(triggered()), this, SLOT(createDocument()));
    connect(ui->aAddNode, SIGNAL(triggered()), activeDocument, SLOT(addNode()));
    connect(ui->aQuit, SIGNAL(triggered()), this, SLOT(quit()));

    settings->endGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    exit(0);
}

void MainWindow::createDocument() {
    DocWindow* docwindow = new DocWindow(mdiArea);
    this->activeDocument = docwindow->getDocument();
    this->activeDocument->resize(settings->value("Size").toSize());
}
