#include "loadconfigdialog.h"
#include "ui_loadconfigdialog.h"

LoadConfigDialog::LoadConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadConfigDialog)
  , selection("")
{
    ui->setupUi(this);
    connect(ui->configList, &QListWidget::itemClicked, this, [=](QListWidgetItem *item) {
        selection = item->text();
    });
}

LoadConfigDialog::~LoadConfigDialog()
{
    delete ui;
}

void LoadConfigDialog::populateList(QStringList fileNames) {
    ui->configList->addItems(fileNames);
}

QString LoadConfigDialog::getSelection() {
    return selection;
}
