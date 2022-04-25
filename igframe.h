#ifndef IGFRAME_H
#define IGFRAME_H

#include <QObject>
#include <QCanBusFrame>
#include <QUuid>

class IGFrame : public QCanBusFrame
{
public:
    IGFrame();
    explicit IGFrame(QUuid uuid, quint32 identifier, bool periodic, quint32 period, const QByteArray &data);
    void print();
    bool isPeriodic();
    quint32 getPeriod() const;
    QString getUuid() const;
    void setPeriodic(bool periodic);
    void setPeriod(quint32 period);

    bool operator==(const IGFrame& rhs)
    {
        if(this->getUuid().compare(rhs.getUuid()) == 0)
            return true;
        return false;
    }

private:
    bool m_periodic = false;
    quint32 m_period = 0;
    QUuid m_uuid;
};

#endif // IGFRAME_H
