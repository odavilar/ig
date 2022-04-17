#include "igtableframe.h"
#include <QDebug>

IGTableFrame::IGTableFrame(QObject *parent) : QObject(parent)
{
    m_data = new QByteArray();
}

IGTableFrame::IGTableFrame(qint32 identifier, bool periodic, float cycle, const QByteArray &data, QObject *parent) :
    QObject(parent), m_identifier(identifier), m_periodic(periodic), m_cycle(cycle)
{
    m_data = new QByteArray(data);
}

void IGTableFrame::print()
{
    qDebug()<<"Identifier: "<< QString::number( m_identifier, 16 ).toUpper();
    qDebug()<<"Periodic: "<<m_periodic;
    qDebug()<<"Cycle: "<<m_cycle;
    qDebug()<<"Data: "<< m_data->toHex();
}

//IGTableFrame::~IGTableFrame()
//{

//}
