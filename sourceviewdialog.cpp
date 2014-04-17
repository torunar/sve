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
    this->view->setHtml(this->htmlTemplate.replace("<!--#VHDL#-->", src));
}
