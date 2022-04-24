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
    qint32 getPeriod() const;
    QString getUuid() const;
    void setPeriodic(bool periodic);
    void setCycle(qint32 cycle);

    bool operator==(const IGFrame& rhs)
    {
        if(this->getUuid().compare(rhs.getUuid()) == 0)
            return true;
        return false;
    }

private:
    bool m_periodic = false;
    qint32 m_cycle = 0;
    QUuid m_uuid;


};

#endif // IGFRAME_H
