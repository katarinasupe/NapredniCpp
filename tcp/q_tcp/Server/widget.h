#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QByteArray>
#include <unordered_map>

class QTcpServer;
class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void onNewConnection();
    void onDisconnectPushed();
    void onReadyRead(QTcpSocket *);
    void onDisconnected(QTcpSocket *);

private:
    Ui::Widget *ui;
    QTcpServer * m_server;
    std::vector<QTcpSocket *> m_sockets;
    std::unordered_map<QTcpSocket * , QByteArray> m_data;
    std::unordered_map<QTcpSocket * , QString> m_usernames;
};
#endif // WIDGET_H
