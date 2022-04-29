#include "config.h"
#include <QFile>
#include <QJsonDocument>

Config::Config()
{

}

Config::Config(QString filename): m_fileName(filename)
{

}

int Config::write()
{
    QFile saveFile(m_fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return -1;
    }

    saveFile.write(QJsonDocument(m_config).toJson());
}

int Config::read()
{


    return 0;
}
