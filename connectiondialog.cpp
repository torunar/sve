#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConnectionDialog) {
    ui->setupUi(this);
}

ConnectionDialog::ConnectionDialog(QList<UNode *> nodes, QWidget *parent) : QDialog(parent), ui(new Ui::ConnectionDialog) {
    ui->setupUi(this);

    QVector<QString> inputs = ((ElementNode *)nodes.last())->getPlugin()->getInputs();
    QVector<QString> outputs = ((ElementNode *)nodes.first())->getPlugin()->getOutputs();
    uint i = this->inCounter;
    for(QVector<QString>::Iterator inputName = inputs.begin(); inputName < inputs.end(); inputName++) {
        QString inputText = (*inputName).replace("%INC%", QString::number(++i));
        new QListWidgetItem(inputText, this->ui->inList);
    }
    i = this->outCounter;
    for(QVector<QString>::Iterator outputName = outputs.begin(); outputName < outputs.end(); outputName++) {
        QString outputText = (*outputName).replace("%OUTC%", QString::number(++i));
        new QListWidgetItem(outputText, this->ui->outList);
    }

    connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this->ui->okButton,     SIGNAL(clicked()), this, SLOT(getSelection()));
}

ConnectionDialog::~ConnectionDialog() {
    delete ui;
}

void ConnectionDialog::setCounters(uint inCounter, uint outCounter) {
    this->inCounter  = inCounter;
    this->outCounter = outCounter;
}

void ConnectionDialog::getSelection() {

}
