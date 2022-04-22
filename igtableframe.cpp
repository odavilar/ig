#include "igtableframe.h"
#include <QDebug>

IGTableFrame::IGTableFrame()
{

}

IGTableFrame::IGTableFrame(QUuid uuid, qint32 identifier, bool periodic, qint32 cycle, const QByteArray &data) :  m_periodic(periodic), m_cycle(cycle), m_uuid(uuid)
{
    this->setPayload(data);
    this->setFrameId(identifier);
}

void IGTableFrame::print()
{
    qDebug()<<"Identifier: "<< QString::number( this->frameId(), 16 ).toUpper();
    qDebug()<<"Periodic: "<<m_periodic;
    qDebug()<<"Cycle: "<<m_cycle;
    qDebug()<<"Data: "<< this->payload().toHex();
}

bool IGTableFrame::isPeriodic()
{
    return m_periodic;
}

qint32 IGTableFrame::getCycle()
{
    return m_cycle;
}

QString IGTableFrame::getUuid()
{
    return m_uuid.toString(QUuid::WithoutBraces);
}

void IGTableFrame::setPeriodic(bool periodic)
{
    m_periodic = periodic;
}

void IGTableFrame::setCycle(qint32 cycle){
    m_cycle = cycle;
}
