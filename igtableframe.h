#ifndef IGTABLEFRAME_H
#define IGTABLEFRAME_H

#include <QObject>
#include <QCanBusFrame>

class IGTableFrame : public QCanBusFrame
{
public:
    explicit IGTableFrame(qint32 identifier, bool periodic, qint32 cycle, const QByteArray &data);
    void print();
    bool isPeriodic();
    qint32 getCycle();

private:
    bool m_periodic = false;
    qint32 m_cycle = 0;
};

#endif // IGTABLEFRAME_H
