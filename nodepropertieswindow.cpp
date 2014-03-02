#include "nodepropertieswindow.h"
#include "ui_nodepropertieswindow.h"

NodePropertiesWindow::NodePropertiesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodePropertiesWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->okButton, SIGNAL(clicked()), SLOT(save()));
}

NodePropertiesWindow::~NodePropertiesWindow() {
    delete ui;
}

void NodePropertiesWindow::setCounters(uint inCounter, uint outCounter) {
    this->inCounter  = inCounter;
    this->outCounter = outCounter;
}

void NodePropertiesWindow::setSource(QString source) {
    for (int i = 1; i <= this->inputs.size(); i++) {
        source.replace("%IN_"  + QString::number(i) + "%", inputs[i-1]);
    }
    for (int i = 1; i <= this->outputs.size(); i++) {
        source.replace("%OUT_" + QString::number(i) + "%", outputs[i-1]);
    }
    ui->source->setText(source);
}

void NodePropertiesWindow::setInputs(QVector<QString> inputs) {
    uint i = this->inCounter;
    for(QVector<QString>::Iterator inputName = inputs.begin(); inputName < inputs.end(); inputName++) {
        QString inputText = (*inputName).replace("%INC%", QString::number(++i));
        this->inputs << inputText;
        new QListWidgetItem(inputText, ui->inList);
    }
}

void NodePropertiesWindow::setOutputs(QVector<QString> outputs) {
    uint i = this->outCounter;
    for(QVector<QString>::Iterator outputName = outputs.begin(); outputName < outputs.end(); outputName++) {
        QString outputText = (*outputName).replace("%OUTC%", QString::number(++i));
        this->outputs << outputText;
        new QListWidgetItem(outputText, ui->outList);
    }
}

void NodePropertiesWindow::save() {
    qDebug() << ui->source->toPlainText();
    this->close();
}
