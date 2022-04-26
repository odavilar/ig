#include "canbusmgr.h"

CANBusMgr::CANBusMgr()
{
    m_PeriodicFrames = QSharedPointer<IGHash>(new IGHash());
    m_thread = new QThread();
    CANBusWorker* worker = new CANBusWorker(&m_PeriodicFrames);
    worker->moveToThread(m_thread);
    connect( worker, &CANBusWorker::error, this, &CANBusMgr::errorString);
    connect( m_thread, &QThread::started, worker, &CANBusWorker::process);
    connect( worker, &CANBusWorker::finished, m_thread, &QThread::quit);
    //connect( worker, &MeasurementWorker::finished, worker, &MeasurementWorker::deleteLater);
    //connect( m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(this, &CANBusMgr::stopMeasurementThread, worker,  &CANBusWorker::stopMeasurement);
    //connect(this, &CANMgr::framesUpdated, worker,  &MeasurementWorker::framesUpdated);
    connect(this, &CANBusMgr::frameUpdated, worker,  &CANBusWorker::frameUpdated);
    connect(this, &CANBusMgr::frameDeleted, worker,  &CANBusWorker::frameDeleted);
}

CANBusMgr::~CANBusMgr()
{
    m_PeriodicFrames->clear();
}

int CANBusMgr::connectDevice(const Settings &p, QString * resultString)
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
        for (const CANBusMgr::ConfigurationItem &item : p.configurations)
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

int CANBusMgr::busStatus()
{
    return 0;
}

void CANBusMgr::disconnectDevice()
{
    if (!m_canDevice)
        return;

    m_canDevice->disconnectDevice();
}

int CANBusMgr::sendFrame(IGFrame * frame)
{
    if(!m_canDevice)
        return -1;

    m_canDevice->writeFrame(*frame);
    return 0;
}

void CANBusMgr::updateFrame(IGFrame frame)
{
    if(frame.isPeriodic())
    {
        QMutexLocker mutex(m_PeriodicFrames->getMutex());
        quint32 period = m_PeriodicFrames->value(frame.getUuid()).getPeriod();
        m_PeriodicFrames->insert(frame.getUuid(), frame);
        emit frameUpdated(frame.getUuid(), period);
    }
    else
    {
        if(m_PeriodicFrames->contains(frame.getUuid()))
            deleteFrame(frame.getUuid());
    }

}

void CANBusMgr::deleteFrame(QString uuid)
{
    QMutexLocker mutex(m_PeriodicFrames->getMutex());
    qint32 period = m_PeriodicFrames->value(uuid).getPeriod();
    if(!m_PeriodicFrames->remove(uuid))
        qDebug()<<"Error deleting";
    emit frameDeleted(uuid, period);
}

void CANBusMgr::startMeasurement()
{
    if(!m_thread->isRunning())
        m_thread->start();
}

void CANBusMgr::stopMeasurement()
{
    emit stopMeasurementThread();
}

void CANBusMgr::errorString(QString err)
{
    qCritical()<<err;
}

bool CANBusMgr::isRunning()
{
    return m_thread->isRunning();
}
