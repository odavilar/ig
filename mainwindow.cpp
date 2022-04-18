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

    initConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnections()
{
    connect(m_connectDialog, &QDialog::accepted, this, &MainWindow::connectDevice);

    connect(m_connectMenuAction, &QAction::triggered, [this]() {
        m_connectDialog->show();
    });

    connect(m_igTable, SIGNAL(sendButtonClicked(IGTableFrame*)), this, SLOT(sendButtonClicked(IGTableFrame*)));
}

void MainWindow::connectDevice()
{
    QString resultString;

    const ConnectDialog::Settings p = m_connectDialog->settings();
    CANMgr::Settings canSettings;
\
    canSettings.configurations = p.configurations;

    for( auto config : p.configurations)
    {
        canSettings.configurations.append(p.configurations);
    }

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

void MainWindow::sendButtonClicked(IGTableFrame *frame)
{
    frame->print();
    m_canmgr->sendFrame(frame);
}
