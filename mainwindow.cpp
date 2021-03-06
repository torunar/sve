#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    /* load language */
    QTranslator *t = new QTranslator();
    t->load("sve_ru.qm");
    qApp->installTranslator(t);

    this->settings = new QSettings("mike-schekotov", "sve");
    ui->setupUi(this);

    /* load css */
    QFile cssFile(":/css/ui-css");
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

    connect(ui->aNew,      SIGNAL(triggered()), this, SLOT(createDocument()));
    connect(ui->aQuit,     SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->aOpen,     SIGNAL(triggered()), this, SLOT(load()));
    connect(ui->aPlugins,  SIGNAL(triggered()), this, SLOT(showPluginListWindow()));
    connect(ui->aSettings, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));
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

void MainWindow::disconnectSlots() {
    disconnect(ui->aAddLabel,    SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
    disconnect(ui->aAddNode,     SIGNAL(triggered()), this->activeWindow, SLOT(addNode()));
    disconnect(ui->aAddLink,     SIGNAL(triggered()), this->activeWindow, SLOT(addLink()));
    disconnect(ui->aSave,        SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    disconnect(ui->aSaveAs,      SIGNAL(triggered()), this->activeWindow, SLOT(saveAs()));
    disconnect(ui->aOptions,     SIGNAL(triggered()), this->activeWindow, SLOT(showOptionsDialog()));
    disconnect(ui->aExportImage, SIGNAL(triggered()), this->activeWindow, SLOT(showSaveImageDialog()));
    disconnect(ui->aViewVHDL,    SIGNAL(triggered()), this->activeWindow, SLOT(viewVHDL()));

    disconnect(ui->aUndo,        SIGNAL(triggered()), this->activeDocument, SLOT(undo()));
}

void MainWindow::connectSlots() {
    connect(ui->aAddLabel,    SIGNAL(triggered()), this->activeWindow, SLOT(addLabel()));
    connect(ui->aAddNode,     SIGNAL(triggered()), this->activeWindow, SLOT(addNode()));
    connect(ui->aAddLink,     SIGNAL(triggered()), this->activeWindow, SLOT(addLink()));
    connect(ui->aSave,        SIGNAL(triggered()), this->activeWindow, SLOT(save()));
    connect(ui->aSaveAs,      SIGNAL(triggered()), this->activeWindow, SLOT(saveAs()));
    connect(ui->aOptions,     SIGNAL(triggered()), this->activeWindow, SLOT(showOptionsDialog()));
    connect(ui->aExportImage, SIGNAL(triggered()), this->activeWindow, SLOT(showSaveImageDialog()));
    connect(ui->aViewVHDL,    SIGNAL(triggered()), this->activeWindow, SLOT(viewVHDL()));

    connect(ui->aUndo,        SIGNAL(triggered()), this->activeDocument, SLOT(undo()));
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

void MainWindow::showPreferencesDialog() {
    PreferencesDialog *pd = new PreferencesDialog();
    if (pd->exec() == QDialog::Accepted) {
        ui->statusBar->showMessage(tr("Restart required for changes to apply"), 0);
    }
    delete pd;
}

// close opened documents and quit
void MainWindow::quit() {
    this->mdiArea->closeAllSubWindows();
    QMdiSubWindow *sb = this->mdiArea->activeSubWindow();
    if (sb == 0) {
        exit(0);
    }
    else {
        sb->activateWindow();
        if (sb->isHidden()) {
            exit(0);
        }
    }
}

// new document
void MainWindow::createDocument() {
    // disconnect old slots
    if (this->activeDocument && this->mdiArea->subWindowList().size() > 0) {
        this->disconnectSlots();
    }
    // create new window
    this->activeWindow = new DocWindow(mdiArea);
    connect(this->activeWindow, SIGNAL(aboutToActivate()), this, SLOT(setActiveDocument()));
    this->activeWindow->attachStatusBar(this->ui->statusBar);
    // set context
    this->activeDocument = this->activeWindow->getDocument();
    this->activeDocument->setPlugins(this->plugins);
    this->connectSlots();

    // resize
    this->activeDocument->resize(settings->value("default_doc/blank_size").toSize());
}

// switching between tabs
void MainWindow::setActiveDocument() {
    // disconnect old slots
    if (this->activeDocument) {
        this->disconnectSlots();
    }
    // switch context
    this->activeWindow = ((DocWindow*) this->sender());
    this->activeDocument = this->activeWindow->getDocument();
    this->connectSlots();
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
    if (pw->exec() == QDialog::Accepted) {
        this->initPluginsToolbar();
    }
}

