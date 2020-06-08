#pragma once
#include <QObject>
#include <QString>
#include <QVariant>
#include <QSettings>
#include <yaml-cpp/yaml.h>

class Configurator : public QObject {
    Q_OBJECT
    enum ProxyType
    {
        SS,
        SSR,
        VMESS,
        TROJAN
    };

    Configurator() = default;

    QSettings config;

public:
    static Configurator& instance();

    static const QString getClashConfigPath();
    static YAML::Node loadClashConfig();

    const QVariant loadValue(const QString& key, const QVariant& defaultValue = {});
    void saveValue(const QString& key, const QVariant& value);

    QList<QStringList> getSubscribes();
    void setSubscribes(const QList<QStringList>& subscribes);
};
