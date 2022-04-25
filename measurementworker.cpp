#include "measurementworker.h"
#include "canmgr.h"
#include <QTime>
#include <QList>

MeasurementWorker::MeasurementWorker(QSharedPointer<IGHash> *frames, QObject *parent)
    : QObject{parent}, m_isRunning(false)
{
    m_PeriodicFrames = *frames;
}

MeasurementWorker::~MeasurementWorker() { // Destructor
    // free resources
}

void MeasurementWorker::process() { // Process. Start processing data.

    qint32 period = 0;
    m_isRunning = true;
    for(auto& frame: *m_PeriodicFrames)
    {
        period = frame.getPeriod();
        if(!m_TimerList.contains(period))
            m_TimerList.insert(period, new QTimer(this));

        if(!m_FramesList.contains(period))
        {
            QList<QString> * list = new QList<QString>();
            list->append(frame.getUuid());
            m_FramesList.insert(period, list);
        }
        else
        {
            QList<QString> * list = m_FramesList.value(period);
            if(!list->contains(frame.getUuid()))
                list->append(frame.getUuid());
        }
    }

    QHashIterator<qint32, QTimer*> i(m_TimerList);
    while (i.hasNext()) {
        i.next();
        qDebug()<< "Starting timer with period "<< i.key();
        i.value()->start(i.key());
        connect(i.value(), &QTimer::timeout, this, &MeasurementWorker::timeoutExpired);
    }
}


void MeasurementWorker::stopMeasurement()
{
    m_isRunning = false;
    QHashIterator<qint32, QTimer*> i(m_TimerList);
    while (i.hasNext()) {
        i.next();
        QTimer * timer = i.value();
        timer->stop();
        delete timer;
    }

    m_TimerList.clear();

    emit finished();
}

void MeasurementWorker::timeoutExpired()
{
    QTimer *timer = dynamic_cast<QTimer*>(QObject::sender());
    int period = timer->interval();
    QString debugPrint;
    QList<QString> * list = m_FramesList.value(period);

    debugPrint = QTime::currentTime().toString("hh:mm:ss.zzz") + " Period: " + QString::number(period);

    for(auto& uuid: *list)
    {
        if(m_PeriodicFrames->contains(uuid))
        {
            IGFrame frame = m_PeriodicFrames->value(uuid);
            debugPrint = debugPrint + " : " + QString::number(frame.frameId(), 16);
        }
    }

    qDebug()<<debugPrint;
}

void MeasurementWorker::frameUpdated(QString uuid)
{
    QTimer * timer = nullptr;

    if(m_PeriodicFrames->contains(uuid))
    {
        if(this->isRunning())
        {
            IGFrame frame = m_PeriodicFrames->value(uuid);
            quint32 period = frame.getPeriod();
            if(period == 0)
                return;

            if(!m_TimerList.contains(period))
            {
                timer = new QTimer(this);
                m_TimerList.insert(period, timer);
                timer->start(period);
                connect(timer, &QTimer::timeout, this, &MeasurementWorker::timeoutExpired);
            }

            if(!m_FramesList.contains(period))
            {
                QList<QString> * list = new QList<QString>();
                list->append(frame.getUuid());
                m_FramesList.insert(period, list);
            }
            else
            {
                QList<QString> * list = m_FramesList.value(period);
                if(!list->contains(frame.getUuid()))
                    list->append(frame.getUuid());
            }
        }
    }
}

void MeasurementWorker::frameDeleted(QString uuid, qint32 period)
{
    QList<QString> * list = m_FramesList.value(period);
    if(!list)
        return;

    list->removeOne(uuid);
    if(list->empty())
    {
        QTimer * timer = m_TimerList.value(period);
        timer->stop();
        delete timer;
        m_TimerList.remove(period);
        delete list;
        m_FramesList.remove(period);
    }
}


bool MeasurementWorker::isRunning()
{
    return m_isRunning;
}
