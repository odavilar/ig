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
    explicit MeasurementWorker(QObject *parent = nullptr);
    ~MeasurementWorker();

public slots:
    void process();
    void stopMeasurement();
    void timeoutExpired();
    void framesUpdated(QMultiMap<qint32, IGTableFrame> *frames);

signals:
    void finished();
    void error(QString err);

private:
    QTimer *m_timer = nullptr;
    QMultiMap<qint32, IGTableFrame> m_frames;
    QMutex m_mutex;
};

#endif // MEASUREMENTWORKER_H
