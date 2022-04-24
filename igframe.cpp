#include "igframe.h"
#include <QDebug>

IGFrame::IGFrame()
{

}

IGFrame::IGFrame(QUuid uuid, qint32 identifier, bool periodic, qint32 cycle, const QByteArray &data) :  m_periodic(periodic), m_cycle(cycle), m_uuid(uuid)
{
    this->setPayload(data);
    this->setFrameId(identifier);
}

void IGFrame::print()
{
    qDebug()<<"Identifier: "<< QString::number( this->frameId(), 16 ).toUpper();
    qDebug()<<"Periodic: "<<m_periodic;
    qDebug()<<"Cycle: "<<m_cycle;
    qDebug()<<"Data: "<< this->payload().toHex();
}

bool IGFrame::isPeriodic()
{
    return m_periodic;
}

qint32 IGFrame::getCycle()
{
    return m_cycle;
}

QString IGFrame::getUuid() const
{
    return m_uuid.toString(QUuid::WithoutBraces);
}

void IGFrame::setPeriodic(bool periodic)
{
    m_periodic = periodic;
}

void IGFrame::setCycle(qint32 cycle){
    m_cycle = cycle;
}
