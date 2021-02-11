#include "widget.h"
#include "ui_widget.h"
#include <QAbstractSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <algorithm>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), m_server(new QTcpServer(this))
{
    ui->setupUi(this);
    ui->disconnectButton->setEnabled(false);
    if(!m_server->listen(QHostAddress(QHostAddress::LocalHost), 9090)){
        ui->serverLog->appendPlainText(tr("server error = %1").arg(m_server->serverError()));
        return;
    }

    connect(m_server, &QTcpServer::newConnection, this, &Widget::onNewConnection);
}


void Widget::onNewConnection()
{
    while(m_server->hasPendingConnections()){
            ui->disconnectButton->setEnabled(true);
            QTcpSocket * socket = m_server->nextPendingConnection();
            m_sockets.push_back(socket);
            ui->serverLog->appendPlainText(QString("Ostvarena konekcija sa %1, port %2").arg(socket->peerAddress().toString(), QString::number(socket->peerPort())));

            connect(ui->disconnectButton, &QPushButton::clicked, this, &Widget::onDisconnectPushed);
            connect(socket, &QTcpSocket::readyRead, this, [this, socket](){onReadyRead(socket);});
            connect(socket, &QTcpSocket::disconnected, this, [this, socket](){onDisconnected(socket);});

            socket->write("Daj mi username!");
    }
}

void Widget::onDisconnectPushed()
{
    //prekid svih uspostavljenih veza
    for(auto const& socket: m_sockets) {
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            socket->close(); //emitira signal disconnected, koji ce eliminirati pripadni socket
        }
    }
    ui->disconnectButton->setEnabled(false);
}

void Widget::onReadyRead(QTcpSocket * socket)
{
    const char ETB = 23; //End-of-Transmission-Block
    if(!socket){
        return;
     }

    QByteArray temp = socket->readAll();
    QString temp_message = QString::fromUtf8(temp);
    if(temp_message.left(3) == "999"){
        QString username = temp_message.remove(0, 3);
        m_usernames.insert({socket, username});
        return;
    }

    QByteArray & buffer = m_data[socket];
    buffer.append(temp);

    //citanje poruke
    while(true){
        //nalazi poziciju ETB, ako postoji (ne postoji ako smo dobili samo dio poruke)
        auto ETB_pos = buffer.indexOf(ETB);
        if(ETB_pos < 0)
            break;
        //uzimamo prvih ETB_pos procitanih znakova (s left(ETB_pos)) koji su u Utf8 i castamo u QString - on koristi Utf16
        QString message = QString::fromUtf8(buffer.left(ETB_pos));
        //praznimo buffer jer smo ga ucitali u message
        buffer.remove(0, ETB_pos + 1);
        ui->serverLog->appendPlainText(tr("%1 : %2").arg(m_usernames[socket], message));

        message.insert(0, " : ");
        message.insert(0, m_usernames[socket]);
        message.insert(0, "<span style=\"color: green\">");
        message.insert(message.length(), "</span>");
        //vracamo iz QString u QByteArray, tj. iz Utf16 u Utf8
        QByteArray poruka = message.toUtf8();
        poruka.append(ETB);
        //slanje poruka svim klijentima
        for(QTcpSocket * klijent : m_sockets){
            if(klijent != socket && klijent->state() == QAbstractSocket::ConnectedState){
                klijent->write(poruka);
            }
        }
    }
}

void Widget::onDisconnected(QTcpSocket * socket)
{
    if(!socket)
            return;
    auto it = std::find(m_sockets.begin(), m_sockets.end(), socket);
    //ako je socket naden u listi socketa
    if(it != m_sockets.end()){
        m_sockets.erase(it);
    }
    auto it_d = m_data.find(socket);
    if(it_d != m_data.end())
        m_data.erase(it_d);
    ui->serverLog->appendPlainText(QString("Prekinuta konekcija sa %1, port %2").arg(socket->peerAddress().toString(), QString::number(socket->peerPort())));
    socket->deleteLater();
}


Widget::~Widget()
{
    delete ui;
}
