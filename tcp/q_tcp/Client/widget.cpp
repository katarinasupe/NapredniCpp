#include "widget.h"
#include "ui_widget.h"

#include <QHostAddress>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), m_socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    ui->disconnectButton->setEnabled(false);
    ui->messageInput->setEnabled(false);

    connect(ui->connectButton, &QPushButton::clicked, this, &Widget::onConnectPushed);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &Widget::onDisconnectPushed);
    connect(m_socket, &QTcpSocket::connected, this, &Widget::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Widget::onDisconnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &Widget::onErrorOccurred);
    connect(m_socket, &QTcpSocket::readyRead, this, &Widget::onReadyRead);


    connect(ui->messageInput, &QLineEdit::returnPressed, this, &Widget::onReturnPressed);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::onConnectPushed()
{
    //ako vec nije connected
    if(m_socket->state() != QAbstractSocket::ConnectedState)
    {
        m_socket->connectToHost(ui->addressLineEdit->text(), ui->portSpinbox->value());
        ui->messagesLog->append("Connecting ...");
        m_name = ui->userLineEdit->text().trimmed();
    }
}


void Widget::onDisconnectPushed()
{
    ui->messagesLog->append("Disconnecting ...");
    m_socket->close();
}

void Widget::onReturnPressed()
{
    const char ETB = 23; // End-of-Transmission-Block
    QString message = ui->messageInput->text().trimmed();
    QByteArray poruka = QString("%1").arg(message).toUtf8();
    poruka.append(ETB);

    m_socket->write(poruka);
    ui->messagesLog->append(QString("<span style=\"color: blue\">Me : %1</span>").arg(message));
    ui->messageInput->clear();
}

void Widget::onConnected()
{
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->addressLineEdit->setEnabled(false);
    ui->userLineEdit->setEnabled(false);
    ui->portSpinbox->setEnabled(false);
    ui->messageInput->setEnabled(true);
    ui->messagesLog->append("Connected.");
}

void Widget::onReadyRead()
{
    const char ETB = 23; // End-of-Transmission-Block

    if(!m_socket)
        return;

    QByteArray temp = m_socket->readAll();
    QString temp_message = QString::fromUtf8(temp);

    if(temp_message.contains("Daj mi username!")){
        QString outgoing_message = "999";
        outgoing_message.append(m_name);
        m_socket->write(outgoing_message.toUtf8());
        return;
    }

    m_data.append(temp);
    while(true){
        auto ETB_pos = m_data.indexOf(ETB);
        if(ETB_pos < 0)
            break;
        QString message = QString::fromUtf8(m_data.left(ETB_pos));
        m_data.remove(0,ETB_pos+1);
        ui->messagesLog->append(message);

    }


}

void Widget::onDisconnected()
{
    ui->messagesLog->append("Disconnected by server.");
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->addressLineEdit->setEnabled(true);
    ui->userLineEdit->setEnabled(true);
    ui->portSpinbox->setEnabled(true);
    ui->messageInput->setEnabled(false);
}

void Widget::onErrorOccurred()
{
    ui->messagesLog->append(tr("Error occurred : %1").arg(m_socket->errorString()));
}

