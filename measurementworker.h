#ifndef MEASUREMENTWORKER_H
#define MEASUREMENTWORKER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMutex>

#include "igtableframe.h"

class MeasurementWorker : public QObject
{
    Q_OBJECT
public:
    explicit MeasurementWorker(QSharedPointer<QMultiMap<qint32, IGTableFrame>> *frames,QObject *parent = nullptr);
    ~MeasurementWorker();

public slots:
    void process();
    void stopMeasurement();
    void timeoutExpired();
    void framesUpdated();

signals:
    void finished();
    void error(QString err);

private:
    QTimer *m_timer = nullptr;
    QSharedPointer<QMultiMap<qint32, IGTableFrame>> m_frames;
    QMutex m_mutex;
    QMap<qint32, QList<IGTableFrame>> m_framesList;
    QMap<qint32, QTimer> m_timerList;
};

#endif // MEASUREMENTWORKER_H
