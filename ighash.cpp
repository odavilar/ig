#include "ighash.h"
#include <QDebug>

IGHash::IGHash()
{

}

void IGHash::lock(){m_mutex.lock();}
void IGHash::unlock(){m_mutex.unlock();}
QMutex * IGHash::getMutex(){return &m_mutex;}
