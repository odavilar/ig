#ifndef CANMGR_H
#define CANMGR_H

#include <QObject>
#include <QTimer>
#include <QCanBusFrame>
#include <QCanBusDevice>
#include <QCanBus>
#include <QVariant>
#include <QThread>
#include <QMutexLocker>

#include <memory>
#include "igtableframe.h"
#include "measurementworker.h"

class CANMgr : public QObject
{
    Q_OBJECT
public:
    typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;

    struct Settings {
        QString pluginName;
        QString deviceInterfaceName;
        QList<ConfigurationItem> configurations;
        bool useConfigurationEnabled = false;
    };

    CANMgr();
    ~CANMgr();
    int connectDevice(const Settings &p, QString * resultString);
    int busStatus();
    void disconnectDevice();
    int sendFrame(IGTableFrame * frame);
    int updatePeriodicFrames(QSharedPointer<QHash<QString, IGTableFrame>> frames);
    void startMeasurement();
    void stopMeasurement();
    void errorString(QString err);
    void getPeriodicFrames(QSharedPointer<QMultiMap<qint32, IGTableFrame>> frames);

signals:
    void stopMeasurementThread();
    void framesUpdated();

private:
    std::unique_ptr<QCanBusDevice> m_canDevice;
    QThread* m_thread = nullptr;
    QList<QTimer> * m_TimerList;
    QSharedPointer<QMultiMap<qint32, IGTableFrame>> m_PeriodicFrames;
    qint64 m_numberFramesWritten = 0;
    QMutex m_mutex;
};

#endif // CANMGR_H
