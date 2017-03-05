#include "passworddialog.h"
#include "ui_passworddialog.h"
#include <QDebug>

passwordDialog::passwordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwordDialog)
{
    ui->setupUi(this);
}

passwordDialog::~passwordDialog()
{
    delete ui;
}

void passwordDialog::on_buttonBox_accepted()
{
    if(ui->passwordField->text().compare(PASSWORD) == 0)
        emit checkPassword(true);
    else
        emit checkPassword(false);
}
