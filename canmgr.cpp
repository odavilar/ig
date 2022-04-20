#include "canmgr.h"

CANMgr::CANMgr()
{
    m_TimerList = new QList<QTimer>();
    m_PeriodicFrames = new QMultiMap<qint32, IGTableFrame>();
}


CANMgr::~CANMgr()
{
    delete m_TimerList;
    m_PeriodicFrames->clear();
    delete m_PeriodicFrames;
}

int CANMgr::connectDevice(const Settings &p, QString * resultString)
{
    int error = 0;
    QString errorString;

    m_canDevice.reset(QCanBus::instance()->createDevice(p.pluginName, p.deviceInterfaceName,
                                                        &errorString));
    if (!m_canDevice) {
        *resultString = tr("Error creating device '%1', reason: '%2'").arg(p.pluginName).arg(errorString);
        return -1;
    }

    m_numberFramesWritten = 0;

    if (p.useConfigurationEnabled) {
        for (const CANMgr::ConfigurationItem &item : p.configurations)
            m_canDevice->setConfigurationParameter(item.first, item.second);
    }

    if (!m_canDevice->connectDevice()) {
        *resultString = tr("Connection error: %1").arg(m_canDevice->errorString());
        m_canDevice.reset();
        return -1;
    } else {

        const QVariant bitRate = m_canDevice->configurationParameter(QCanBusDevice::BitRateKey);
        if (bitRate.isValid()) {
            const bool isCanFd =
                    m_canDevice->configurationParameter(QCanBusDevice::CanFdKey).toBool();
            const QVariant dataBitRate =
                    m_canDevice->configurationParameter(QCanBusDevice::DataBitRateKey);
            if (isCanFd && dataBitRate.isValid()) {
                 *resultString = tr("Plugin: %1, connected to %2 at %3 / %4 kBit/s")
                                  .arg(p.pluginName).arg(p.deviceInterfaceName)
                                  .arg(bitRate.toInt() / 1000).arg(dataBitRate.toInt() / 1000);
            } else {
                 *resultString = tr("Plugin: %1, connected to %2 at %3 kBit/s")
                                  .arg(p.pluginName).arg(p.deviceInterfaceName)
                                  .arg(bitRate.toInt() / 1000);
            }
        } else {
             *resultString = tr("Plugin: %1, connected to %2")
                    .arg(p.pluginName).arg(p.deviceInterfaceName);
        }

    }

    return error;
}

int CANMgr::busStatus()
{
    return 0;
}

void CANMgr::disconnectDevice()
{
    if (!m_canDevice)
        return;

    m_canDevice->disconnectDevice();
}

int CANMgr::sendFrame(IGTableFrame * frame)
{
    if(!m_canDevice)
        return -1;

    m_canDevice->writeFrame(*frame);
    return 0;
}

int CANMgr::updatePeriodicFrames(QList<IGTableFrame> *frames)
{
    m_PeriodicFrames->clear(); // Will it destroy all inner objects?

    for(auto& frame : *frames)
        m_PeriodicFrames->insert(frame.getCycle(), frame);

    return 0;
}
