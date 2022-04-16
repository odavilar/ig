#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCanBusDevice>
#include <QLabel>

class ConnectDialog;
class IGTable;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectDevice();
    void busStatus();
    void disconnectDevice();

private:
    Ui::MainWindow *ui;
    ConnectDialog *m_connectDialog = nullptr;
    QAction * m_connectMenuAction = nullptr;
    IGTable * m_igTable = nullptr;

    qint64 m_numberFramesWritten = 0;
    Ui::MainWindow *m_ui = nullptr;
    QLabel *m_status = nullptr;
    QLabel *m_written = nullptr;
    std::unique_ptr<QCanBusDevice> m_canDevice;
    QTimer *m_busStatusTimer = nullptr;

    void initConnections();

public slots:
    void sendButton();
};
#endif // MAINWINDOW_H
