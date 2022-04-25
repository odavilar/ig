#ifndef CANBUSMGR_H
#define CANBUSMGR_H

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
#include "canbusworker.h"

class CANBusMgr : public QObject
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

    CANBusMgr();
    ~CANBusMgr();
    int connectDevice(const Settings &p, QString * resultString);
    int busStatus();
    void disconnectDevice();
    void startMeasurement();
    void stopMeasurement();
    void errorString(QString err);
    bool isRunning();

signals:
    void stopMeasurementThread();
    void frameUpdated(QString uuid);
    void frameDeleted(QString uuid, qint32 period);

public slots:
    void updateFrame(IGFrame frame);
    void deleteFrame(QString uuid);
    int sendFrame(IGFrame * frame);

private:
    std::unique_ptr<QCanBusDevice> m_canDevice;
    QThread* m_thread = nullptr;
    QSharedPointer<IGHash> m_PeriodicFrames;
    qint64 m_numberFramesWritten = 0;
};

#endif // CANBUSMGR_H
