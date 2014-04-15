#include "documentoptionsdialog.h"
#include "ui_documentoptionsdialog.h"

DocumentOptionsDialog::DocumentOptionsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DocumentOptionsDialog) {
    ui->setupUi(this);

    connect(this->ui->okButton,     SIGNAL(clicked()), this, SLOT(accept()));
    connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

DocumentOptionsDialog::~DocumentOptionsDialog() {
    delete ui;
}

void DocumentOptionsDialog::setDimensions(QSize size) {
    this->ui->heightSpinner->setValue(size.height());
    this->ui->widthSpinner->setValue(size.width());
}

QSize DocumentOptionsDialog::getDimensions() {
    return QSize(this->ui->widthSpinner->value(), this->ui->heightSpinner->value());
}
