#include "mainwindow.h"

#include <QFormLayout>
#include <QPushButton>

#include <QApplication>
#include <QAction>
#include <QMessageBox>
#include <QStandardItem>



void MainWindow::createIcons()
{
    lightbulbIcon = QPixmap(":/Ikone/lightbulb.png");
    windowIcon = QIcon(":/Ikone/window.png");
}

void MainWindow::setupCoreWidgets()
{
    //glavni grafički element na koji se postavljaju svi ostali
    mainWidget = new QWidget();

    centralWidgetLayout = new QVBoxLayout();
    formLayout = new QGridLayout();

    //graficki elementi za unos
    nameLabel = new QLabel("Name:");
    passwordLabel = new QLabel("Password:");

    //imamo dva line edita, za ime i lozinku
    nameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();

    //password skriven tockicama
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    //dodajemo ih u form layout
    formLayout->addWidget(nameLabel, 0,0);
    formLayout->addWidget(nameLineEdit, 0,1);
    formLayout->addWidget(passwordLabel, 1,0);
    formLayout->addWidget(passwordLineEdit, 1,1);

    //dodavanje gornjeg dijela sucelja
    centralWidgetLayout->addLayout(formLayout);

    appModel = new QStandardItemModel(1,2,this);
    appModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    appModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Password")));

    //imamo dva gumba, jedan za cancel, drugi za submit -> postavljamo ih u QHBoxLayout razmještaj
    buttonsLayout = new QHBoxLayout();
    cancelButton = new QPushButton("Cancel");
    submitButton = new QPushButton("Submit");

    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(submitButton);

    centralWidgetLayout->addLayout(buttonsLayout);

    failureMessageBox = new QMessageBox(QMessageBox::NoIcon, "Failure", "Try again.", QMessageBox::Ok, this);
    failureMessageBox->setIconPixmap(lightbulbIcon);
    successMessageBox = new QMessageBox(QMessageBox::NoIcon, "Success", "You have successfully logged in.", QMessageBox::Ok, this);
    successMessageBox->setIconPixmap(lightbulbIcon);
}

void MainWindow::setupSignalsAndSlots()
{
    connect(cancelButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitButtonClicked);
}

void MainWindow::submitButtonClicked()
{
    QString inputName = nameLineEdit->text();
    QString inputPassword = passwordLineEdit->text();

    if(!inputName.isEmpty() || !inputPassword.isEmpty()){
        if(inputName == name && inputPassword == password)
        {
            successLogin();
        }
        else
        {
            clearFields();
            failureLogin();
        }
    }
}

void MainWindow::clearFields()
{
    nameLineEdit->clear();
    passwordLineEdit->clear();
}

void MainWindow::successLogin()
{
    successMessageBox->exec();
    QApplication::quit();
}

void MainWindow::failureLogin(){
    failureMessageBox->exec();
}


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    name = "katarina";
    password = "03071995";
    setWindowTitle("qt_login"); //ova metoda je nasljedena iz QWindowMain klase
    resize(300, 100);

    createIcons();
    setWindowIcon(windowIcon);
    setupCoreWidgets();
    setupSignalsAndSlots();

    mainWidget->setLayout(centralWidgetLayout);
    //mainWidget postaje dijete MainWindow objekta
    setCentralWidget(mainWidget);

}

MainWindow::~MainWindow()
{
}
