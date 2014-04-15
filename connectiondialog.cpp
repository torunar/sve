#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConnectionDialog) {
    ui->setupUi(this);
}

ConnectionDialog::ConnectionDialog(QList<UNode *> nodes, QWidget *parent) : QDialog(parent), ui(new Ui::ConnectionDialog) {
    ui->setupUi(this);

    QVector<QString> inputs = ((ElementNode *)nodes.last())->getPlugin()->getInputs();
    QVector<QString> outputs = ((ElementNode *)nodes.first())->getPlugin()->getOutputs();
    uint i = 0;
    for(QVector<QString>::Iterator inputName = inputs.begin(); inputName < inputs.end(); inputName++) {
        QString inputText = (*inputName).replace("%INC%", QString::number(++i));
        new QListWidgetItem(inputText, this->ui->inList);
    }
    i = 0;
    for(QVector<QString>::Iterator outputName = outputs.begin(); outputName < outputs.end(); outputName++) {
        QString outputText = (*outputName).replace("%OUTC%", QString::number(++i));
        new QListWidgetItem(outputText, this->ui->outList);
    }

    this->selectedInput  = -1;
    this->selectedOutput = -1;

    connect(this->ui->cancelButton, SIGNAL(clicked()),              this, SLOT(reject()));
    connect(this->ui->okButton,     SIGNAL(clicked()),              this, SLOT(accept()));
    connect(this->ui->inList,       SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(setInput(QListWidgetItem *)));
    connect(this->ui->outList,      SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(setOutput(QListWidgetItem *)));
}

ConnectionDialog::~ConnectionDialog() {
    delete ui;
}

void ConnectionDialog::setCounters(uint inCounter, uint outCounter) {
    this->inCounter  = inCounter;
    this->outCounter = outCounter;
}

QPair<int, int> ConnectionDialog::getConnectors() {
    return QPair<int, int>(this->selectedOutput, this->selectedInput);
}

void ConnectionDialog::setConnectors(QPair<int, int> connectors) {
    this->selectedOutput = connectors.first;
    this->selectedInput  = connectors.second;
    this->ui->outList->setCurrentRow(connectors.first);
    this->ui->inList->setCurrentRow(connectors.second);
    this->ui->okButton->setEnabled(true);
}

void ConnectionDialog::setInput(QListWidgetItem *) {
    QModelIndexList l = this->ui->inList->selectionModel()->selectedIndexes();
    foreach(QModelIndex i, l) {
        this->selectedInput = i.row();
    }
    if ((selectedInput != -1) && (selectedOutput != -1)) {
        this->ui->okButton->setEnabled(true);
    }
}

void ConnectionDialog::setOutput(QListWidgetItem *) {
    QModelIndexList l = this->ui->outList->selectionModel()->selectedIndexes();
    foreach(QModelIndex i, l) {
        this->selectedOutput = i.row();
    }
    if ((selectedInput != -1) && (selectedOutput != -1)) {
        this->ui->okButton->setEnabled(true);
    }
}
