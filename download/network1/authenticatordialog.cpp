#include "authenticatordialog.h"
#include "ui_authenticatordialog.h"
#include <QPushButton>

authenticatorDialog::authenticatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authenticatorDialog)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

authenticatorDialog::~authenticatorDialog()
{
    delete ui;
}

QString authenticatorDialog::get_username()
{
    return ui->usernameLineEdit->text();
}

QString authenticatorDialog::get_password()
{
    return ui->passwordLineEdit->text();
}
