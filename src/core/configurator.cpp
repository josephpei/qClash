#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonObject>

#include "configurator.h"

Configurator &Configurator::instance()
{
    static Configurator configuratorInstance;
    return configuratorInstance;
}

const QString Configurator::getClashConfigPath()
{
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return homePath + "/.config/clash/";
}

YAML::Node Configurator::loadClashConfig()
{
    QString configFile = Configurator::getClashConfigPath() + "config.yaml";
    YAML::Node root = YAML::LoadFile(configFile.toStdString());
    return root;
}

const QVariant Configurator::loadValue(const QString &key, const QVariant &defaultValue)
{
    return config.value(key, defaultValue);
}

void Configurator::saveValue(const QString &key, const QVariant &value)
{
    config.setValue(key, value);
    config.sync();
}

QList<Subscribe> Configurator::getSubscribes() {
    QList<QString> data = loadValue("subscribes").value<QList<QString>>();
    QList<Subscribe> subscribes;
    for (int i = 0; i < data.size(); i++) {
        subscribes.append(Subscribe(stringToJson(data[i])));
    }
    return subscribes;
}

void Configurator::setSubscribes(const QList<Subscribe> &subscribes) {
    QList<QString> data;
    for (int i = 0; i < subscribes.size(); ++i) {
        data.append(jsonToString(subscribes[i].write()));
    }
    saveValue("subscribes", QVariant::fromValue(data));
}
