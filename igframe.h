#ifndef IGFRAME_H
#define IGFRAME_H

#include <QObject>
#include <QCanBusFrame>
#include <QUuid>

class IGFrame : public QCanBusFrame
{
public:
    IGFrame();
    explicit IGFrame(QUuid uuid, qint32 identifier, bool periodic, qint32 cycle, const QByteArray &data);
    void print();
    bool isPeriodic();
    qint32 getCycle();
    QString getUuid();
    void setPeriodic(bool periodic);
    void setCycle(qint32 cycle);

private:
    bool m_periodic = false;
    qint32 m_cycle = 0;
    QUuid m_uuid;
};

#endif // IGFRAME_H
