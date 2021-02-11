#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "authenticatordialog.h"
#include <QMainWindow>
class QNetworkReply; //manje vrijeme kompilacije nego #include
class QNetworkAccessManager;
class QAuthenticator;
class QProgressBar;
class QTemporaryFile;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onFinished();
    void onReadyRead();
    void onDownloadClicked();
    void onAuthenticationRequired(QNetworkReply *, QAuthenticator *);
    void onDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void onClearClicked();
    void onExitClicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager * nam;
    QNetworkReply * reply;
    QProgressBar * progress;
    QTemporaryFile * temp_file;
    authenticatorDialog * dialog = new authenticatorDialog();
    void makeRequest(QUrl const & url);
    bool isText;

};
#endif // MAINWINDOW_H
