#include "nodepropertiesdialog.h"
#include "ui_nodepropertiesdialog.h"

NodePropertiesDialog::NodePropertiesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NodePropertiesDialog) {
    ui->setupUi(this);
    this->setFixedSize(this->size());

    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->okButton, SIGNAL(clicked()), SLOT(save()));
}

NodePropertiesDialog::~NodePropertiesDialog() {
    delete ui;
}

void NodePropertiesDialog::setSource(QString source) {
    for (int i = 1; i <= this->inputs.size(); i++) {
        source.replace("%IN_"  + QString::number(i) + "%", inputs[i-1]);
    }
    for (int i = 1; i <= this->outputs.size(); i++) {
        source.replace("%OUT_" + QString::number(i) + "%", outputs[i-1]);
    }
    ui->source->setText(source);
}

void NodePropertiesDialog::setInputs(QVector<QString> inputs) {
    int i = 0;
    for(QVector<QString>::Iterator inputName = inputs.begin(); inputName < inputs.end(); inputName++) {
        QString inputText = (*inputName).replace("%INC%", QString::number(++i));
        this->inputs << inputText;
        new QListWidgetItem(inputText, ui->inList);
    }
}

void NodePropertiesDialog::setOutputs(QVector<QString> outputs) {
    int i = 0;
    for(QVector<QString>::Iterator outputName = outputs.begin(); outputName < outputs.end(); outputName++) {
        QString outputText = (*outputName).replace("%OUTC%", QString::number(++i));
        this->outputs << outputText;
        new QListWidgetItem(outputText, ui->outList);
    }
}

void NodePropertiesDialog::save() {
    this->close();
}
