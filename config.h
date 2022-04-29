#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonObject>

class Config
{
public:
    Config();
    Config(QString filename);
    int save(QJsonObject * config);
    int load(QJsonObject * config);

private:
    int write();
    int read();
    QString m_fileName;
    QJsonObject m_config;
};

#endif // CONFIG_H
