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

void NodePropertiesWindow::setSource(QString source) {
    ui->source->setText(source);
}

void NodePropertiesWindow::setInputs(QVector<QString> *inputs) {
    for(QVector<QString>::Iterator inputName = inputs->begin(); inputName < inputs->end(); inputName++) {
        new QListWidgetItem(*inputName, ui->inList);
    }
}

void NodePropertiesWindow::setOutputs(QVector<QString> *outputs) {
    for(QVector<QString>::Iterator outputName = outputs->begin(); outputName < outputs->end(); outputName++) {
        new QListWidgetItem(*outputName, ui->outList);
    }
}

void NodePropertiesWindow::save() {
    qDebug() << ui->source->toPlainText();
    this->close();
}
