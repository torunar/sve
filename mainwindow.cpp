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
    this->mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    this->setCentralWidget(mdiArea);

    this->activeDocument = NULL;
    this->activeWindow = NULL;

    this->createDocument();

    connect(ui->aNew, SIGNAL(triggered()), this, SLOT(createDocument()));
    connect(ui->aQuit, SIGNAL(triggered()), this, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    for (int i = this->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder).length() - 1; i >= 0; i--) {
        int before = this->mdiArea->subWindowList().length();
        ((DocWindow*)this->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder).at(i))->close();
        int after = this->mdiArea->subWindowList().length();
        // closing unsaved document aborted with "cancel"
        if (after == before) return;
    }
    if (this->mdiArea->subWindowList().length() == 0) {
        exit(0);
    }
}

void MainWindow::createDocument() {
    // disconnect old slots
    if (this->activeDocument) {
        disconnect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
        disconnect(ui->aSave, SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    }
    // create new window
    this->activeWindow = new DocWindow(mdiArea);
    connect(this->activeWindow, SIGNAL(aboutToActivate()), this, SLOT(setActiveDocument()));
    // set context
    this->activeDocument = this->activeWindow->getDocument();
    connect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
    connect(ui->aSave, SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    // resize
    this->activeDocument->resize(settings->value("Size").toSize());
}

void MainWindow::setActiveDocument() {
    // disconnect old slots
    if (this->activeDocument) {
        disconnect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
        disconnect(ui->aSave, SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    }
    // switch context
    this->activeWindow = ((DocWindow*) this->sender());
    this->activeDocument = this->activeWindow->getDocument();
    // connect new slots
    connect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
    connect(ui->aSave, SIGNAL(triggered()), this->activeWindow, SLOT(save()));
}

void MainWindow::closeEvent(QCloseEvent *ev){
    ev->ignore();
    this->quit();
}

