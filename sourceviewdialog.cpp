#include "sourceviewdialog.h"
#include "ui_sourceviewdialog.h"

SourceViewDialog::SourceViewDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SourceViewDialog) {
    ui->setupUi(this);

    this->view = new QWebView();
    this->ui->gridLayout->addWidget(this->view, 0, 0, 1, 1);
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
    this->view->setHtml(
        this->htmlTemplate
            .replace("<!--#CSS#-->",  css.readAll())
            .replace("<!--#JS#-->",   js.readAll())
            .replace("<!--#VHDL#-->", src)
            .replace("<!--#MAIN#-->", main.readAll())
    );
    css.close();
    js.close();
    main.close();
}
