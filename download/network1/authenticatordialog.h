#ifndef AUTHENTICATORDIALOG_H
#define AUTHENTICATORDIALOG_H

#include <QDialog>
class MyOkButton;
class MyCancelButton;

namespace Ui {
class authenticatorDialog;
}

class authenticatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit authenticatorDialog(QWidget *parent = nullptr);
    ~authenticatorDialog();
    QString get_username();
    QString get_password();

private:
    Ui::authenticatorDialog *ui;
    QString username;
    QString password;
};

#endif // AUTHENTICATORDIALOG_H
