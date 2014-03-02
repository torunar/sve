#include "addnodedialog.h"
#include "ui_addnodedialog.h"

AddNodeDialog::AddNodeDialog(QStringList plugins, QWidget *parent) : QDialog(parent), ui(new Ui::AddNodeDialog) {
    ui->setupUi(this);
    foreach(QString plugin, plugins) {
        this->ui->pluginList->addItem(plugin);
    }
    connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this->ui->pluginList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(unlockAdd()));
    connect(this->ui->pluginList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(getSelection()));
    connect(this->ui->addButton, SIGNAL(clicked()), this, SLOT(getSelection()));
}

AddNodeDialog::~AddNodeDialog()
{
    delete ui;
}

void AddNodeDialog::getSelection(){
    QString selectedItem = "";
    if (this->ui->pluginList->selectedItems().size() > 0) {
        selectedItem = this->ui->pluginList->selectedItems().first()->text();
    }
    emit(itemSelected(selectedItem));
    this->accept();
}

void AddNodeDialog::unlockAdd() {
    this->ui->addButton->setEnabled(true);
}
