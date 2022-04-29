#ifndef CANBUSWORKER_H
#define CANBUSWORKER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMutex>
#include <QCanBusDevice>

#include "igframe.h"
#include "ighash.h"

class CANBusWorker : public QObject
{
    Q_OBJECT

    using period = qint32;

public:
    explicit CANBusWorker(QSharedPointer<IGHash> *frames, QObject *parent = nullptr);
    ~CANBusWorker();

public slots:
    void process();
    void stopMeasurement();
    void timeoutExpired();
    void frameUpdated(QString uuid, quint32 old_period);
    void frameDeleted(QString uuid, quint32 period);
    bool isRunning();
    bool setDevice(QCanBusDevice * device);

signals:
    void finished();
    void error(QString err);

private:
    QCanBusDevice * m_canDevice;
    QSharedPointer<IGHash> m_PeriodicFrames;
    QHash<period, QTimer*> m_TimerList;
    QHash<period, QList<QString>*> m_FramesList;
    bool m_isRunning;
};

#endif // CANBUSWORKER_H
