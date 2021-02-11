#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTableView>
#include <QStandardItemModel>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QIcon>


class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setupCoreWidgets();
    void setupSignalsAndSlots();
    void clearFields();
    void successLogin();
    void failureLogin();
    void createIcons();

    QWidget * mainWidget = nullptr;
    QVBoxLayout * centralWidgetLayout = nullptr;
    QGridLayout * formLayout = nullptr;
    QHBoxLayout * buttonsLayout = nullptr;

    QPushButton * cancelButton;
    QPushButton * submitButton;

    QLineEdit * nameLineEdit = nullptr;
    QLineEdit * passwordLineEdit = nullptr;

    QTableView * appTable = nullptr;
    QStandardItemModel * appModel = nullptr;

    QLabel * nameLabel = nullptr;
    QLabel * passwordLabel = nullptr;

    QString name;
    QString password;

    QIcon windowIcon;
    QPixmap lightbulbIcon;

    QMessageBox * failureMessageBox = nullptr;
    QMessageBox * successMessageBox = nullptr;

private slots:
    void submitButtonClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
