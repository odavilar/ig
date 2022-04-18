#ifndef IGTABLEFRAME_H
#define IGTABLEFRAME_H

#include <QObject>
#include <QCanBusFrame>

class IGTableFrame : public QCanBusFrame
{
public:
    explicit IGTableFrame(qint32 identifier, bool periodic, float cycle, const QByteArray &data);
    void print();

private:
    bool m_periodic = false;
    float m_cycle = 0;
};

#endif // IGTABLEFRAME_H
