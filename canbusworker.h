#ifndef CANBUSWORKER_H
#define CANBUSWORKER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMutex>

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
    void frameUpdated(QString uuid);
    void frameDeleted(QString uuid, qint32 period);
    bool isRunning();

signals:
    void finished();
    void error(QString err);

private:
    QSharedPointer<IGHash> m_PeriodicFrames;
    QHash<period, QTimer*> m_TimerList;
    QHash<period, QList<QString>*> m_FramesList;
    bool m_isRunning;
};

#endif // CANBUSWORKER_H
