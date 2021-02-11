#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authenticatordialog.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QMessageBox>
#include <QAuthenticator>
#include <QProgressBar>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QTemporaryFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), nam(new QNetworkAccessManager(this)), progress(new QProgressBar(this)) //dajemu mu roditelja da se ne moramo brinuti o njegovom brisanju
{
    ui->setupUi(this);
    //dodavanje brisanja urlLineEdita
    ui->urlLineEdit->setClearButtonEnabled(true);
    //naziv prozora
    setWindowTitle("File download");

    connect(ui->downloadPushButton, &QPushButton::clicked, this, &MainWindow::onDownloadClicked);
    connect(nam, &QNetworkAccessManager::authenticationRequired, this, &MainWindow::onAuthenticationRequired);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::onExitClicked);

    //progress bar
    progress->setMinimum(0);
    progress->setMaximum(100);
    progress->setValue(0);
    ui->statusbar->addWidget(progress);
    progress->setVisible(false);
}


void MainWindow::makeRequest(QUrl const & url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "myApplication");

    reply = nam->get(request);

    connect(reply, &QNetworkReply::finished, this, &MainWindow::onFinished);
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &MainWindow::onDownloadProgress);

    progress->setValue(0);
}

void MainWindow::onFinished()
{
    //ako postoji error
    if(reply->error() != QNetworkReply::NoError)
    {
        ui->headersPlainText->textCursor().insertText(reply->errorString());
        ui->plainTextEdit->textCursor().insertText(reply->readAll());
        ui->statusbar->showMessage(tr("HTTP status = %1").arg(
                                      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() ));
        return;
    }

    //napuni header
    auto list = reply->rawHeaderPairs();
    for(auto item : list)
    {
        ui->headersPlainText->appendPlainText(item.first + " : " + item.second);
    }

    QVariant target = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);


    if(target.isValid())
    {
        QUrl new_url = target.toUrl();
        auto button = QMessageBox::question(this, "Redirection", tr("Do you want to redirect to %1").arg(new_url.toString()));
        if(button == QMessageBox::No)
            return; //redirekcija se ne napravi, server prekine vezu i gotovi smo
        makeRequest(new_url); //inace se serveru salje novi zahtjev
    }

    QTemporaryFile temp_file;
    temp_file.open();
    temp_file.write(reply->readAll());
    temp_file.flush();
    temp_file.seek(0);

    if(isText)
    {
        ui->plainTextEdit->textCursor().insertText(temp_file.readAll());
    }
    else{
        QString fileName = QFileDialog::getSaveFileName(this, tr("Spremi datoteku"));
        ui->plainTextEdit->textCursor().insertText("Saved to " + fileName);
        QFile::copy(temp_file.fileName(), fileName);
    }

    reply->deleteLater();
}

void MainWindow::onReadyRead(){
    //provjera je li text
    if(reply->header(QNetworkRequest::ContentTypeHeader).toString().left(4) == "text")
    {
        isText = true;
    }
    else{
        isText = false;
        progress->setVisible(true);
    }
}

void MainWindow::onDownloadClicked()
{
    auto url_text = ui->urlLineEdit->text().trimmed();
    if(url_text.isEmpty())
        return;

    QUrl url(url_text);
    makeRequest(url);
}


void MainWindow::onAuthenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator)
{
    if(dialog->exec() == QDialog::Accepted)
    {
        authenticator->setUser(dialog->get_username());
        authenticator->setPassword(dialog->get_password());
    }
    //ako se autentikacija preskoci, tj if-naredba se preskoci, onda se preusmjeri na onFinish, pa ne moramo raditi nista
}

//long int broj bitova koje smo dobili i totalni broj bitova -> racunanje postotka za postavljanje vrijednosti u progress baru
void MainWindow::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qreal progress_ = (bytesTotal < 1) ? 1.0 : static_cast<qreal>(bytesSent)/bytesTotal;
    progress->setValue(progress_ * progress->maximum());
}

void MainWindow::onClearClicked()
{
    ui->statusbar->clearMessage();
    progress->setVisible(false);
    ui->urlLineEdit->clear();
    ui->plainTextEdit->clear();
    ui->headersPlainText->clear();
}

void MainWindow::onExitClicked()
{
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}



