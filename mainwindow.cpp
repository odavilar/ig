#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "igtable.h"
#include "QVBoxLayout"
#include "serialbus/connectdialog.h"
#include <QCanBus>

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
    const ConnectDialog::Settings p = m_connectDialog->settings();

    QString errorString;
    m_canDevice.reset(QCanBus::instance()->createDevice(p.pluginName, p.deviceInterfaceName,
                                                        &errorString));
    if (!m_canDevice) {
        m_status->setText(tr("Error creating device '%1', reason: '%2'")
                          .arg(p.pluginName).arg(errorString));
        return;
    }

    m_numberFramesWritten = 0;

    if (p.useConfigurationEnabled) {
        for (const ConnectDialog::ConfigurationItem &item : p.configurations)
            m_canDevice->setConfigurationParameter(item.first, item.second);
    }

    if (!m_canDevice->connectDevice()) {
        m_status->setText(tr("Connection error: %1").arg(m_canDevice->errorString()));

        m_canDevice.reset();
    } else {

        const QVariant bitRate = m_canDevice->configurationParameter(QCanBusDevice::BitRateKey);
        if (bitRate.isValid()) {
            const bool isCanFd =
                    m_canDevice->configurationParameter(QCanBusDevice::CanFdKey).toBool();
            const QVariant dataBitRate =
                    m_canDevice->configurationParameter(QCanBusDevice::DataBitRateKey);
            if (isCanFd && dataBitRate.isValid()) {
                m_status->setText(tr("Plugin: %1, connected to %2 at %3 / %4 kBit/s")
                                  .arg(p.pluginName).arg(p.deviceInterfaceName)
                                  .arg(bitRate.toInt() / 1000).arg(dataBitRate.toInt() / 1000));
            } else {
                m_status->setText(tr("Plugin: %1, connected to %2 at %3 kBit/s")
                                  .arg(p.pluginName).arg(p.deviceInterfaceName)
                                  .arg(bitRate.toInt() / 1000));
            }
        } else {
            m_status->setText(tr("Plugin: %1, connected to %2")
                    .arg(p.pluginName).arg(p.deviceInterfaceName));
        }

    }
}


void MainWindow::disconnectDevice()
{
    if (!m_canDevice)
        return;

    m_canDevice->disconnectDevice();

    m_status->setText(tr("Disconnected"));
}

void MainWindow::sendButtonClicked(IGTableFrame *frame)
{
    qDebug()<<"Just in case";
    frame->print();
}
