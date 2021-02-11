#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>

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
    void onConnectPushed();
    void onDisconnectPushed();
    void onReturnPressed();
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred();


private:
    Ui::Widget *ui;
    QTcpSocket * m_socket;
    QByteArray m_data;
    QString m_name;
};
#endif // WIDGET_H
