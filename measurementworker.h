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
public:
    explicit MeasurementWorker(QSharedPointer<IGHash> *frames,QObject *parent = nullptr);
    ~MeasurementWorker();

public slots:
    void process();
    void stopMeasurement();
    void timeoutExpired();
    void framesUpdated();
    void frameUpdated(QString uuid);
    void frameDeleted(QString uuid);

signals:
    void finished();
    void error(QString err);

private:
    QTimer *m_timer = nullptr;
    QSharedPointer<IGHash> m_PeriodicFrames;
    QMutex m_mutex;
    QMap<qint32, QTimer> m_timerList;
};

#endif // MEASUREMENTWORKER_H
