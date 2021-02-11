#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setupCoreWidgets();
    setupSignalsAndSlots();
}

void MainWindow::setupCoreWidgets()
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //ne dozvoljavamo edit u tablici

    tableModel = new QStandardItemModel(1, 3, this);
    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Ime i prezime")));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Email")));
    tableModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Napomena")));

    QRegularExpression commaRegEx("[^,]+");
    QValidator *commaValidator = new QRegularExpressionValidator(commaRegEx, this);

    QRegularExpression emailRegEx("[\\s]*[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}[\\s]*", QRegularExpression::CaseInsensitiveOption);
    QValidator *emailValidator = new QRegularExpressionValidator(emailRegEx, this);

    ui->iipLineEdit->setValidator(commaValidator);
    ui->emailLineEdit->setValidator(emailValidator);
    ui->napomenaLineEdit->setValidator(commaValidator);

    QStandardItem * first = new QStandardItem(QString("Katarina Supe"));
    QStandardItem * second = new QStandardItem(QString("kasupe@student.math.hr"));
    QStandardItem * third = new QStandardItem(QString("Racunarstvo"));

    tableModel->setItem(0, 0, first);
    tableModel->setItem(0, 1, second);
    tableModel->setItem(0, 2, third);

    ui->tableView->setModel(tableModel);
}


void MainWindow::setupSignalsAndSlots()
{
    connect(ui->actionQuit, &QAction::triggered, &QApplication::quit);
    connect(ui->savePushButton, &QPushButton::clicked, this, &MainWindow::saveButtonClicked);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutDialog);
    connect(ui->ClearAllPushButton, &QPushButton::clicked, this, &MainWindow::clearAllRecords);
    connect(ui->actionDeleteAll, &QAction::triggered, this, &MainWindow::clearAllRecords);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::deleteSavedRecord);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(this, &MainWindow::fileOpenedSignal, ui->statusbar, &QStatusBar::showMessage);
}

void MainWindow::clearFields()
{
    ui->iipLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->napomenaLineEdit->clear();
}

void MainWindow::saveButtonClicked()
{
    QString name = ui->iipLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString napomena = ui->napomenaLineEdit->text();

    if(!name.isEmpty() || !email.isEmpty()){ //dozvoljavamo da napomena bude prazna
        QStandardItem * it1{new QStandardItem(name)};
        QStandardItem * it2{new QStandardItem(email)};
        QStandardItem * it3{new QStandardItem(napomena)};
        tableModel->appendRow({it1, it2, it3});
        clearFields();
        isFileChanged = true;
    }
}


void MainWindow::openFile()
{
    //u aplikaciji se nalaze podaci iz neke druge datoteke te je bilo promjena
    if(!currentFilePath.isEmpty() && isFileChanged)
    {
        //pitaj korisnika zeli li spremiti nove promjene u postojecu datoteku
        auto saveChangesButton = QMessageBox::question(this, "Spremanje promjena", "Zelite li spremiti promjene?");
        if(saveChangesButton == QMessageBox::Yes)
        {
            save();
            isFileChanged = false; //sad je datoteka nanovo spremljena, pa nema novih promjena
        }
        //ako nije bilo promjena, nema potrebe za ponovnim spremanjem
    }
    //postoje neki podaci u tablici koji nisu uzeti iz datoteke -> slucaj nakon pokretanja programa
    else if(currentFilePath.isEmpty() && tableModel->rowCount() > 0)
    {
        auto saveChangesButton = QMessageBox::question(this, "Spremanje promjena", "Zelite li spremiti promjene?");
        if(saveChangesButton == QMessageBox::Yes)
        {
            saveAs();
            isFileChanged = false; //sad je datoteka nanovo spremljena, pa nema novih promjena
        }
    }

    //otvaranje nove .txt datoteke
    filePath = QFileDialog::getOpenFileName(
                            this,
                            "Odaberite datoteku",
                            QDir::currentPath(),
                            "Text files (*.txt)");
    if(filePath.isEmpty())
        return; //ako nismo odabrali nijednu novu .txt datoteku, nemoj zatvarati staru

    //inace, obrisi stari sadrzaj -> u novoj tabeli su samo podaci iz novootvorene datoteke
    tableModel->removeRows(0, tableModel->rowCount());

    QFile file(filePath);
    file.open( QIODevice::ReadWrite | QIODevice::Text );
    currentFilePath = filePath;

    //ucitavanje redaka tablice
    QTextStream in( &file );
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList row = line.split(",");

        QStandardItem * it1{new QStandardItem(row.at(0))};
        QStandardItem * it2{new QStandardItem(row.at(1))};
        QStandardItem * it3{new QStandardItem("")};

        if(row.length() > 2)
            it3 = new QStandardItem(row.at(2));

        tableModel->appendRow({it1, it2, it3});
    }

    QFileInfo info(filePath);

    isFileChanged = false; //otvorili smo novu datoteku u kojoj na pocetku nema nikakvih promjena
    emit fileOpenedSignal(info.fileName());
}

void MainWindow::clearAllRecords()
{
    auto button = QMessageBox::question(this, "Obrisi sve", "Zelite li bas sve obrisati?!");
    if(button == QMessageBox::Yes)
    {
        tableModel->removeRows(0, tableModel->rowCount());
        isFileChanged = true;
    }
}

void MainWindow::deleteSavedRecord()
{
    bool ok = true;
    int row = QInputDialog::getInt(this, tr("Odaberite red koji cete obrisati"), tr("ID reda"), 1, 1, tableModel->rowCount(), 1, &ok);
    if(ok)
    {
        tableModel->removeRow(row - 1);
        isFileChanged = true;
    }
}


void MainWindow::saveAs()
{
    QString destinationPath = QFileDialog::getSaveFileName(this, tr("Spremi datoteku"), tr("*.txt"));
    QFile unsavedFile(destinationPath);

    unsavedFile.open( QIODevice::WriteOnly | QIODevice::Text );
    currentFilePath = destinationPath;

   QTextStream out( &unsavedFile );
   for(int i = 0; i < tableModel->rowCount(); ++i){

       out << tableModel->data(tableModel->index(i, 0, QModelIndex())).toString() << ","
           << tableModel->data(tableModel->index(i, 1, QModelIndex())).toString() << ","
           << tableModel->data(tableModel->index(i, 2, QModelIndex())).toString();

       if(i != tableModel->rowCount())
           out << "\n";
   }

   QFileInfo info(destinationPath);
   isFileChanged = false;
   emit fileOpenedSignal(info.fileName());
}

void MainWindow::save()
{
    if(currentFilePath.isEmpty())
    {
        saveAs();
    }
    else if(isFileChanged){
        QFile newFile(currentFilePath);

        newFile.open( QIODevice::ReadWrite| QIODevice::Text );
        newFile.resize(0);

        QTextStream out( &newFile );
        for(int i = 0; i < tableModel->rowCount(); ++i){

            out << tableModel->data(tableModel->index(i, 0, QModelIndex())).toString() << ","
                << tableModel->data(tableModel->index(i, 1, QModelIndex())).toString() << ","
                << tableModel->data(tableModel->index(i, 2, QModelIndex())).toString();

            if(i != tableModel->rowCount())
                out << "\n";
        }
        isFileChanged = false;
    }
    //ako nije bilo promjena ne radi nista
}

void MainWindow::aboutDialog()
{
    QMessageBox::about(this, "O aplikaciji", "Katarinina aplikacija");
}

MainWindow::~MainWindow()
{
    delete ui;
}

