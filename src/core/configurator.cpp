#include <QStandardPaths>

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

QList<QStringList> Configurator::getSubscribes() {
    return loadValue("subscribes").value<QList<QStringList>>();
}

void Configurator::setSubscribes(const QList<QStringList> &subscribes) {
    saveValue("subscribes", QVariant::fromValue(subscribes));
}
