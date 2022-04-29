#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCanBusDevice>
#include <QLabel>
#include <QToolBar>
#include "igframe.h"
#include "canbusmgr.h"
#include "ighash.h"

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

    QMenu *m_fileMenu = nullptr;
    QMenu *m_deviceMenu = nullptr;
    QToolBar *m_toolBar = nullptr;

    QAction * m_openConfigAction = nullptr;
    QAction * m_saveConfigAction = nullptr;
    QAction * m_exitAction = nullptr;
    QAction * m_connectMenuAction = nullptr;
    QAction * m_disconnectMenuAction = nullptr;
    QAction * m_startMenuAction = nullptr;
    QAction * m_stopMenuAction = nullptr;

    ConnectDialog *m_connectDialog = nullptr;
    IGTable * m_igTable = nullptr;

    Ui::MainWindow *m_ui = nullptr;
    QLabel *m_status = nullptr;
    QLabel *m_written = nullptr;
    std::unique_ptr<QCanBusDevice> m_canDevice;
    QTimer *m_busStatusTimer = nullptr;
    CANBusMgr * m_canmgr = nullptr;

    QString m_curFile;

    void initConnections();
    void createMenu();
    void createToolBar();
    bool save();
    bool saveAs();
    bool saveFile(QString file);
    void open();
};
#endif // MAINWINDOW_H
