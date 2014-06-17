#include "sourceviewdialog.h"
#include "ui_sourceviewdialog.h"

SourceViewDialog::SourceViewDialog(QString vhdlSource, QWidget *parent) : QDialog(parent), ui(new Ui::SourceViewDialog) {
    ui->setupUi(this);

    this->view = new QWebView();
    this->ui->gridLayout->addWidget(this->view, 0, 0, 1, 1);

    this->vhdlSource = vhdlSource;

    connect(this->ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(this->ui->validateButton, SIGNAL(clicked()), this, SLOT(validate()));
}

SourceViewDialog::~SourceViewDialog() {
    delete ui;
}

void SourceViewDialog::setSource(QString src) {
    QFile css(":/css/hl-css");
    QFile js(":/js/hl-js");
    QFile main(":/js/main-js");
    css.open(QFile::ReadOnly);
    js.open(QFile::ReadOnly);
    main.open(QFile::ReadOnly);
    this->view->setHtml(this->htmlTemplate.arg(css.readAll(), js.readAll(), src, main.readAll()));
    css.close();
    js.close();
    main.close();
}

void SourceViewDialog::validate() {
    QTemporaryFile vhdl("sve-XXXXXX.vhdl");
    vhdl.setAutoRemove(true);
    if (vhdl.open()) {
        vhdl.write(this->vhdlSource.toUtf8());
        vhdl.close();
        // params
        QStringList params;
        params << "-s" << vhdl.fileName();
        // process
        QProcess *ghdl = new QProcess();
        ghdl->setProcessChannelMode(QProcess::MergedChannels);
        // error handler
        connect(ghdl, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
        ghdl->start("ghdl", params);
        ghdl->waitForFinished(-1);
        QString output = ghdl->readAllStandardOutput();
        if (output != "") {
            this->ui->log->setText(output);
        }
        else {
            if (this->ghdlNotFound) {
                this->ui->log->setText(tr("Error running GHDL.\nIs it fall yet?"));
            }
            else {
                this->ui->log->setText(tr("Code seems to be OK!"));
            }
        }
    }
    else {
        this->ui->log->setText(tr("Error creating temporary file"));
    }
}

void SourceViewDialog::handleError(QProcess::ProcessError) {
    this->ghdlNotFound = true;
}
