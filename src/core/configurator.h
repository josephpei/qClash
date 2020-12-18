#pragma once
#include <QObject>
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <yaml-cpp/yaml.h>

#include "common.h"


class Configurator : public QObject {
    Q_OBJECT

    Configurator() = default;

    QSettings config;
    YAML::Node root;
    QJsonObject proxyGroups;

public:
    static Configurator& instance();

    static const QString getClashConfigPath();
    static const QString getClashConfigPath(const QString& name);
    static void saveClashConfig(const QString& name, const QString& content);
    
    YAML::Node loadClashConfig(const QString& name);

    const QVariant loadValue(const QString& key, const QVariant& defaultValue = {});
    void saveValue(const QString& key, const QVariant& value);

    QList<Subscribe> getSubscribes();
    void setSubscribes(const QList<Subscribe>& subscribes);

    Subscribe getCurrentConfig();
    void setCurrentConfig(const Subscribe& subscribe);
    Subscribe getSubscribeByName(const QString &name);

    QJsonObject getProxyGroupsRule(const QString& name);
    void setProxyGroupsRule(const QString& name, const QString& group, const QString& proxy);

    QString getHttpPort();
    QString getSocksPort();
    QString getExternalControlPort();
    bool getAllowLan();
    QString getLogLevel();
};
