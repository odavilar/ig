#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "igtable.h"
#include "QVBoxLayout"
#include "serialbus/connectdialog.h"
#include <QCanBus>
#include <QCanBusFrame>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_status = new QLabel;
    ui->statusbar->addPermanentWidget(m_status);

    m_written = new QLabel;
    ui->statusbar->addWidget(m_written);

    m_igTable = new IGTable(this);
    this->setCentralWidget(m_igTable);

    m_canmgr = new CANBusMgr();

    m_connectDialog = new ConnectDialog;

    createMenu();

    initConnections();
}

MainWindow::~MainWindow()
{
    delete m_status;
    delete m_written;
    delete m_igTable;
    delete m_canmgr;
    delete m_connectDialog;
    delete m_connectMenuAction;
    delete m_startMenuAction;
    delete m_stopMenuAction;
    delete ui;
}

void MainWindow::createMenu()
{



    const QIcon openIcon = QIcon(":/icons/open");
    m_openConfigAction = new QAction(openIcon, tr("&Open"), this);
    m_openConfigAction->setStatusTip(tr("Open configuration"));

    const QIcon saveIcon = QIcon(":/icons/save");
    m_saveConfigAction = new QAction(saveIcon, tr("&Save"), this);
    m_saveConfigAction->setStatusTip(tr("Save configuration"));

    m_exitAction = new QAction(tr("&Exit"), this);
    m_exitAction->setStatusTip(tr("Exit"));

    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openConfigAction);
    m_fileMenu->addAction(m_saveConfigAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);



    ui->menubar->addMenu(m_fileMenu);

    const QIcon connectIcon = QIcon(":/icons/connect");
    m_connectMenuAction = new QAction(connectIcon, tr("&Connect"), this);
    m_connectMenuAction->setStatusTip(tr("Connect device"));

    const QIcon disconnectIcon = QIcon(":/icons/disconnect");
    m_disconnectMenuAction = new QAction(disconnectIcon, tr("&Disconnect"), this);
    m_disconnectMenuAction->setStatusTip(tr("Disconnect device"));

    const QIcon startIcon = QIcon(":/icons/start");
    m_startMenuAction = new QAction(startIcon, tr("Start"), this);
    m_startMenuAction->setStatusTip(tr("Start measurement"));

    const QIcon stopIcon = QIcon(":/icons/stop");
    m_stopMenuAction = new QAction(stopIcon, tr("Stop"), this);
    m_stopMenuAction->setStatusTip(tr("Stop measurement"));

    m_deviceMenu = menuBar()->addMenu(tr("&Device"));
    m_deviceMenu->addAction(m_connectMenuAction);
    m_deviceMenu->addAction(m_disconnectMenuAction);
    m_deviceMenu->addSeparator();
    m_deviceMenu->addAction(m_startMenuAction);
    m_deviceMenu->addAction(m_stopMenuAction);

    m_toolBar = addToolBar(tr("File"));
    m_toolBar->addAction(m_openConfigAction);
    m_toolBar->addAction(m_saveConfigAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_connectMenuAction);
    m_toolBar->addAction(m_disconnectMenuAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_startMenuAction);
    m_toolBar->addAction(m_stopMenuAction);
    m_toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    m_startMenuAction->setEnabled(false);
    m_stopMenuAction->setEnabled(false);
    m_disconnectMenuAction->setEnabled(false);

    ui->menubar->addMenu(m_deviceMenu);
}

void MainWindow::initConnections()
{
    connect(m_connectDialog, &QDialog::accepted, this, &MainWindow::connectDevice);

    connect(m_connectMenuAction, &QAction::triggered, [this]() {
        m_connectDialog->show();
    });

    connect(m_disconnectMenuAction, &QAction::triggered, this, &MainWindow::disconnectDevice);

    connect(m_startMenuAction, &QAction::triggered, m_canmgr, [this]() {
        m_canmgr->startMeasurement();
    });

    connect(m_stopMenuAction, &QAction::triggered, m_canmgr, [this]() {
        m_canmgr->stopMeasurement();
    });

    connect(m_saveConfigAction, &QAction::triggered, this, &MainWindow::save);
    connect(m_openConfigAction, &QAction::triggered, this, &MainWindow::open);

    connect(m_igTable, SIGNAL(sendButtonClicked(IGFrame*)), m_canmgr, SLOT(sendFrame(IGFrame*)));
    connect(m_igTable, SIGNAL(updateFrame(IGFrame)), m_canmgr, SLOT(updateFrame(IGFrame)));
    connect(m_igTable, SIGNAL(deleteFrame(QString)), m_canmgr, SLOT(deleteFrame(QString)));
}

void MainWindow::connectDevice()
{
    QString resultString;

    const ConnectDialog::Settings p = m_connectDialog->settings();
    CANBusMgr::Settings canSettings;
\
    canSettings.configurations.append(p.configurations);
    canSettings.deviceInterfaceName = p.deviceInterfaceName;
    canSettings.pluginName = p.pluginName;
    canSettings.useConfigurationEnabled = p.useConfigurationEnabled;

    if(m_canmgr->connectDevice(canSettings, &resultString) == 0)
    {
        m_startMenuAction->setEnabled(true);
        m_stopMenuAction->setEnabled(true);
        m_disconnectMenuAction->setEnabled(true);
        m_connectMenuAction->setEnabled(false);

    }
    m_status->setText(resultString);
}


void MainWindow::disconnectDevice()
{
    m_canmgr->stopMeasurement();
    m_canmgr->disconnectDevice();
    m_status->setText(tr("Disconnected"));
    m_startMenuAction->setEnabled(false);
    m_stopMenuAction->setEnabled(false);
    m_disconnectMenuAction->setEnabled(false);
    m_connectMenuAction->setEnabled(true);
}

bool MainWindow::save()
{
    if (m_curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(m_curFile);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

bool MainWindow::saveFile(QString file)
{
    qDebug()<<file;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        qDebug()<<fileName;
}
