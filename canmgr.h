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
#include "igframe.h"
#include "ighash.h"
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
    int sendFrame(IGFrame * frame);
    void startMeasurement();
    void stopMeasurement();
    void errorString(QString err);

signals:
    void stopMeasurementThread();
    void frameUpdated(QString uuid);
    void frameDeleted(QString uuid, qint32 period);

public slots:
    void updateFrame(IGFrame frame);
    void deleteFrame(QString uuid);

private:
    std::unique_ptr<QCanBusDevice> m_canDevice;
    QThread* m_thread = nullptr;
    QSharedPointer<IGHash> m_PeriodicFrames;
    qint64 m_numberFramesWritten = 0;
};

#endif // CANMGR_H
