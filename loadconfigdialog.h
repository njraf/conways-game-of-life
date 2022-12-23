#ifndef LOADCONFIGDIALOG_H
#define LOADCONFIGDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class LoadConfigDialog;
}

class LoadConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadConfigDialog(QWidget *parent = nullptr);
    ~LoadConfigDialog();
    void populateList(QStringList fileNames);
    QString getSelection();

private:
    Ui::LoadConfigDialog *ui;
    QString selection;
};

#endif // LOADCONFIGDIALOG_H
