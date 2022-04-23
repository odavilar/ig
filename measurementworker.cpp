#include "measurementworker.h"
#include "canmgr.h"
#include <QTime>

MeasurementWorker::MeasurementWorker(QSharedPointer<QMultiMap<qint32, IGFrame>> *frames, QObject *parent)
    : QObject{parent}
{
    m_frames = *frames;
}

MeasurementWorker::~MeasurementWorker() { // Destructor
    // free resources
}

void MeasurementWorker::process() { // Process. Start processing data.
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MeasurementWorker::timeoutExpired);
    m_timer->start(1);
}


void MeasurementWorker::stopMeasurement()
{
    m_timer->stop();
    delete m_timer;
    emit finished();
}

void MeasurementWorker::timeoutExpired()
{
    static int count = 0;
    if(++count % 1000 == 0)
        qDebug()<<QTime::currentTime();
}

void MeasurementWorker::framesUpdated()
{
   // m_frames = *frames;
}
