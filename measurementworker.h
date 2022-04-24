#ifndef MEASUREMENTWORKER_H
#define MEASUREMENTWORKER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMutex>

#include "igframe.h"
#include "ighash.h"

class MeasurementWorker : public QObject
{
    Q_OBJECT

    using period = qint32;

public:
    explicit MeasurementWorker(QSharedPointer<IGHash> *frames,QObject *parent = nullptr);
    ~MeasurementWorker();

public slots:
    void process();
    void stopMeasurement();
    void timeoutExpired();
    void frameUpdated(QString uuid);
    void frameDeleted(QString uuid, qint32 period);

signals:
    void finished();
    void error(QString err);

private:
    QSharedPointer<IGHash> m_PeriodicFrames;
    QHash<period, QTimer*> m_TimerList;
    QHash<period, QList<QString>*> m_FramesList;
};

#endif // MEASUREMENTWORKER_H
