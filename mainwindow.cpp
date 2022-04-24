#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "igtable.h"
#include "QVBoxLayout"
#include "serialbus/connectdialog.h"
#include <QCanBus>
#include <QCanBusFrame>

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

    m_canmgr = new CANMgr();

    m_connectDialog = new ConnectDialog;

    m_connectMenuAction = new QAction(tr("&Connect"), this);
    m_connectMenuAction->setStatusTip(tr("Connect device"));

    ui->menubar->addAction(m_connectMenuAction);

    m_startMenuAction = new QAction(tr("Start"), this);
    m_startMenuAction->setStatusTip(tr("Start measurement"));
    ui->menubar->addAction(m_startMenuAction);

    m_stopMenuAction = new QAction(tr("Stop"), this);
    m_stopMenuAction->setStatusTip(tr("Stop measurement"));
    ui->menubar->addAction(m_stopMenuAction);

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

void MainWindow::initConnections()
{
    connect(m_connectDialog, &QDialog::accepted, this, &MainWindow::connectDevice);

    connect(m_connectMenuAction, &QAction::triggered, [this]() {
        m_connectDialog->show();
    });

    connect(m_startMenuAction, &QAction::triggered, m_canmgr, [this]() {
        m_canmgr->startMeasurement();
    });

    connect(m_stopMenuAction, &QAction::triggered, m_canmgr, [this]() {
        m_canmgr->stopMeasurement();
    });

    connect(m_igTable, SIGNAL(sendButtonClicked(IGFrame*)), this, SLOT(sendButtonClicked(IGFrame*)));

    connect(m_igTable, SIGNAL(updatePeriodicFrames(QSharedPointer<IGHash>*)), this, SLOT(updatePeriodicFrames(QSharedPointer<IGHash>*)));
    connect(m_igTable, SIGNAL(updateFrame(IGFrame)), m_canmgr, SLOT(updateFrame(IGFrame)));
    connect(m_igTable, SIGNAL(deleteFrame(QString)), m_canmgr, SLOT(deleteFrame(QString)));
}

void MainWindow::connectDevice()
{
    QString resultString;

    const ConnectDialog::Settings p = m_connectDialog->settings();
    CANMgr::Settings canSettings;
\
    canSettings.configurations.append(p.configurations);
    canSettings.deviceInterfaceName = p.deviceInterfaceName;
    canSettings.pluginName = p.pluginName;
    canSettings.useConfigurationEnabled = p.useConfigurationEnabled;

    m_canmgr->connectDevice(canSettings, &resultString);
    m_status->setText(resultString);
}


void MainWindow::disconnectDevice()
{
    m_canmgr->disconnectDevice();
    m_status->setText(tr("Disconnected"));
}

void MainWindow::sendButtonClicked(IGFrame *frame)
{
    frame->print();
    m_canmgr->sendFrame(frame);
}

void MainWindow::updatePeriodicFrames(QSharedPointer<IGHash> *periodicFrames)
{
    m_canmgr->updatePeriodicFrames(*periodicFrames);
}
