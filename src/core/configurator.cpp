#include <QStandardPaths>

#include "configurator.h"

Configurator &Configurator::instance()
{
    static Configurator configuratorInstance;
    return configuratorInstance;
}

YAML::Node Configurator::loadClashConfig()
{
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString configFile = homePath + "/.config/clash/config.yaml";
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
