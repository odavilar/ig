#include "canmgr.h"

CANMgr::CANMgr()
{
    m_PeriodicFrames = QSharedPointer<IGHash>(new IGHash());
    m_thread = new QThread();
    MeasurementWorker* worker = new MeasurementWorker(&m_PeriodicFrames);
    worker->moveToThread(m_thread);
    connect( worker, &MeasurementWorker::error, this, &CANMgr::errorString);
    connect( m_thread, &QThread::started, worker, &MeasurementWorker::process);
    connect( worker, &MeasurementWorker::finished, m_thread, &QThread::quit);
    //connect( worker, &MeasurementWorker::finished, worker, &MeasurementWorker::deleteLater);
    //connect( m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(this, &CANMgr::stopMeasurementThread, worker,  &MeasurementWorker::stopMeasurement);
    //connect(this, &CANMgr::framesUpdated, worker,  &MeasurementWorker::framesUpdated);
    connect(this, &CANMgr::frameUpdated, worker,  &MeasurementWorker::frameUpdated);
    connect(this, &CANMgr::frameDeleted, worker,  &MeasurementWorker::frameDeleted);
}

CANMgr::~CANMgr()
{
    m_PeriodicFrames->clear();
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

int CANMgr::sendFrame(IGFrame * frame)
{
    if(!m_canDevice)
        return -1;

    m_canDevice->writeFrame(*frame);
    return 0;
}

void CANMgr::updateFrame(IGFrame frame)
{
    if(frame.isPeriodic())
    {
        QMutexLocker mutex(m_PeriodicFrames->getMutex());
        m_PeriodicFrames->insert(frame.getUuid(), frame);
        emit frameUpdated(frame.getUuid());
    }
    else
    {
        if(m_PeriodicFrames->contains(frame.getUuid()))
            deleteFrame(frame.getUuid());
    }

}

void CANMgr::deleteFrame(QString uuid)
{
    QMutexLocker mutex(m_PeriodicFrames->getMutex());
    qint32 period = m_PeriodicFrames->value(uuid).getPeriod();
    if(!m_PeriodicFrames->remove(uuid))
        qDebug()<<"Error deleting";
    emit frameDeleted(uuid, period);
}

void CANMgr::startMeasurement()
{
    m_thread->start();
}

void CANMgr::stopMeasurement()
{
    emit stopMeasurementThread();
}

void CANMgr::errorString(QString err)
{
    qCritical()<<err;
}

bool CANMgr::isRunning()
{
    return m_thread->isRunning();
}
