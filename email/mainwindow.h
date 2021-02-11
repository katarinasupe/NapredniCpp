#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QStandardItemModel;
class QRegularExpression;
class QValidator;
class QStatusBar;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //ui = user interface -> imenik
    Ui::MainWindow *ui;
    QStandardItemModel * tableModel;
    QString filePath;
    QString currentFilePath;
    QLabel * statusBarLabel;
    bool isFileChanged = false;
    void setupSignalsAndSlots();
    void clearFields();
    void setupCoreWidgets();
    void setupStatusBar();
signals:
    void fileOpenedSignal(const QString &message, int timeout = 0);
private slots:
    void saveButtonClicked();
    void openFile();
    void clearAllRecords();
    void aboutDialog();
    void deleteSavedRecord();
    void saveAs();
    void save();

};
#endif // MAINWINDOW_H
