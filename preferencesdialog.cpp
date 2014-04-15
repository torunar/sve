#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);

    settings = new QSettings("mike-schekotov", "sve");

    QSize docSize  = settings->value("default_doc/blank_size").toSize();
    QSize nodeSize = settings->value("default_doc/node_size").toSize();

    ui->docWidthSpinner->setValue(docSize.width());
    ui->docHeightSpinner->setValue(docSize.height());

    ui->nodeWidthSpinner->setValue(nodeSize.width());
    ui->nodeHeightSpinner->setValue(nodeSize.height());

    ui->pathField->setText(settings->value("plugins/plugin_dir").toString());

    connect(ui->okButton,     SIGNAL(clicked()), this, SLOT(saveOptions()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browsePath()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::browsePath() {
    QString pluginDir = QFileDialog::getExistingDirectory(this, tr("Set plugins directory"), "./");
    if (!pluginDir.isEmpty()) {
        ui->pathField->setText(pluginDir);
    }
}

void PreferencesDialog::saveOptions() {
    settings->setValue("default_doc/blank_size", QSize(ui->docWidthSpinner->value(),  ui->docHeightSpinner->value()));
    settings->setValue("default_doc/node_size",  QSize(ui->nodeWidthSpinner->value(), ui->nodeHeightSpinner->value()));
    settings->setValue("plugins/plugin_dir",     ui->pathField->text());
    this->accept();
}
