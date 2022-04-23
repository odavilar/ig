#ifndef IGHASH_H
#define IGHASH_H

#include <QHash>
#include <QString>
#include <QMutex>
#include "igframe.h"

class IGHash : public QHash<QString, IGFrame>
{
public:
    IGHash();
    void lock();
    void unlock();
    QMutex * getMutex();

private:
    QMutex m_mutex;
};

#endif // IGHASH_H
