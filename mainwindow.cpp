#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->settings = new QSettings("torunar", "sve");
    ui->setupUi(this);

    /* load css */
    // TODO: change path to binary-related
    QFile cssFile("../src/ui.conf");
    cssFile.open(QFile::ReadOnly);
    QString css = cssFile.readAll();
    cssFile.close();
    this->setStyleSheet(css);

    /* set MDI options */
    this->mdiArea = new QMdiArea;
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setViewMode(QMdiArea::TabbedView);
    this->mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    this->setCentralWidget(mdiArea);

    /* set initial states */
    this->activeDocument = NULL;
    this->activeWindow   = NULL;

    /* load plugins in program */
    this->loadPlugins();

    /* create blank document window */
    this->createDocument();

    /* load plugins into sidebar */
    this->initPluginsToolbar();

    connect(ui->aNew,     SIGNAL(triggered()), this, SLOT(createDocument()));
    connect(ui->aQuit,    SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->aOpen,    SIGNAL(triggered()), this, SLOT(load()));
    connect(ui->aPlugins, SIGNAL(triggered()), this, SLOT(showPluginListWindow()));
}

MainWindow::~MainWindow() {
    delete ui;
}

// load all enabled plugins
void MainWindow::loadPlugins() {
    QDir pluginDir(this->settings->value("plugins/plugin_dir").toString());
    QStringList enabledPlugins = this->settings->value("plugins/enabled").toStringList();
    this->plugins = Plugin::loadByList(pluginDir, enabledPlugins);
}

// init sidebar
void MainWindow::initPluginsToolbar() {
    this->ui->pluginBar->clear();
    QStringList panelPlugins = this->settings->value("plugins/on_panel").toStringList();
    foreach(Plugin *p, this->plugins) {
        QAction *a = new QAction(
            p->getPixmap(this->settings->value("plugins/icon_size").toSize()),
            p->getName(),
            0
        );
        a->setObjectName(p->getName());
        connect(a, SIGNAL(triggered()), this, SLOT(findPlugin()));
        if (panelPlugins.contains(p->getName(), Qt::CaseInsensitive)) {
            this->ui->pluginBar->insertAction(0, a);
        }
    }
}

// route sidebar click
void MainWindow::findPlugin() {
    QString activatedPlugin = this->sender()->objectName();
    Plugin *p = this->activeDocument->getPlugin(activatedPlugin);
    this->activeWindow->addNode(p);
}

// close opened documents and quit
void MainWindow::quit() {
    for (int i = this->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder).length() - 1; i >= 0; i--) {
        int before = this->mdiArea->subWindowList().length();
        ((DocWindow*)this->mdiArea->subWindowList(QMdiArea::ActivationHistoryOrder).at(i))->close();
        int after = this->mdiArea->subWindowList().length();
        // closing of unsaved document was aborted with "cancel"
        if (after == before) return;
    }
    if (this->mdiArea->subWindowList().length() == 0) {
        exit(0);
    }
}

// new document
void MainWindow::createDocument() {
    // disconnect old slots
    if (this->activeDocument && this->mdiArea->subWindowList().size() > 0) {
        disconnect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
        disconnect(ui->aSave,     SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    }
    // create new window
    this->activeWindow = new DocWindow(mdiArea);
    connect(this->activeWindow, SIGNAL(aboutToActivate()), this, SLOT(setActiveDocument()));
    // set context
    this->activeDocument = this->activeWindow->getDocument();
    this->activeDocument->setPlugins(this->plugins);

    connect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
    connect(ui->aAddNode,  SIGNAL(triggered()), this->activeWindow, SLOT(addNode()));
    connect(ui->aSave,     SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    // resize
    this->activeDocument->resize(settings->value("default_doc/blank_size").toSize());
}

// switching between tabs
void MainWindow::setActiveDocument() {
    // disconnect old slots
    if (this->activeDocument) {
        disconnect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
        disconnect(ui->aAddNode,  SIGNAL(triggered()), this->activeWindow, SLOT(addNode()));
        disconnect(ui->aSave,     SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    }
    // switch context
    this->activeWindow = ((DocWindow*) this->sender());
    this->activeDocument = this->activeWindow->getDocument();
    // connect new slots
    connect(ui->aAddLabel, SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
    connect(ui->aAddNode,  SIGNAL(triggered()), this->activeWindow, SLOT(addNode()));
    connect(ui->aSave,     SIGNAL(triggered()), this->activeWindow, SLOT(save()));
}

void MainWindow::closeEvent(QCloseEvent *ev){
    ev->ignore();
    this->quit();
}

void MainWindow::load() {
    this->createDocument();
    if (!this->activeWindow->load()) {
        this->activeWindow->close();
    }
}

void MainWindow::showPluginListWindow() {
    PluginListWindow *pw = new PluginListWindow(this);
    pw->show();
    // refresh toolbar
    connect(pw, SIGNAL(finished(int)), this, SLOT(initPluginsToolbar()));
}

