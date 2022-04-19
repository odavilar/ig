#ifndef CANMGR_H
#define CANMGR_H

#include <QObject>
#include <QTimer>
#include <QCanBusFrame>
#include <QCanBusDevice>
#include <QCanBus>
#include <memory>
#include "igtableframe.h"
#include <QVariant>

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
    int updatePeriodicFrames(QList<IGTableFrame> *frames);

private:
    std::unique_ptr<QCanBusDevice> m_canDevice;
    QList<QTimer> * m_TimerList;
    QMultiMap<qint32, IGTableFrame> * m_PeriodicFrames;
    qint64 m_numberFramesWritten = 0;

};

#endif // CANMGR_H
