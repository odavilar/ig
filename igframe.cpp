#include "igframe.h"
#include <QDebug>

IGFrame::IGFrame()
{

}

IGFrame::IGFrame(QUuid uuid, quint32 identifier, bool periodic, quint32 period, const QByteArray &data) :  m_periodic(periodic), m_period(period), m_uuid(uuid)
{
    this->setPayload(data);
    this->setFrameId(identifier);
}

void IGFrame::print()
{
    qDebug()<<"Identifier: "<< QString::number( this->frameId(), 16 ).toUpper();
    qDebug()<<"Periodic: "<<m_periodic;
    qDebug()<<"Cycle: "<<m_period;
    qDebug()<<"Data: "<< this->payload().toHex();
}

bool IGFrame::isPeriodic()
{
    return m_periodic;
}

quint32 IGFrame::getPeriod() const
{
    return m_period;
}

QString IGFrame::getUuid() const
{
    return m_uuid.toString(QUuid::WithoutBraces);
}

void IGFrame::setPeriodic(bool periodic)
{
    m_periodic = periodic;
}

void IGFrame::setPeriod(quint32 period){
    m_period = period;
}
