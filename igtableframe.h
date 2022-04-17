#ifndef IGTABLEFRAME_H
#define IGTABLEFRAME_H

#include <QObject>

class IGTableFrame : public QObject
{
    Q_OBJECT
public:
    explicit IGTableFrame(QObject *parent = nullptr);
    explicit IGTableFrame(qint32 identifier, bool periodic, float cycle, const QByteArray &data, QObject *parent = nullptr);
    //~IGTableFrame();
    void print();
    quint32 getIdentifier();
    QByteArray * getData();

private:
    quint32 m_identifier = 0;
    bool m_periodic = false;
    float m_cycle = 0;
    QByteArray *m_data = nullptr;
};

#endif // IGTABLEFRAME_H
