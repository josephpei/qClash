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

//    QSettings config;
    YAML::Node root;
    QJsonObject proxyGroups;

public:
    static Configurator& instance();

    static QString getAppFilePath();
    static QString getClashConfigPath();
    static QString getClashConfigPath(const QString& name);
    static void saveClashConfig(const QString& name, const QString& content);
    
    YAML::Node loadClashConfig(const QString& name);

    QVariant loadValue(const QString& key, const QVariant& defaultValue = {});
    void saveValue(const QString& key, const QVariant& value);

    QList<Subscribe> getSubscribes();
    void setSubscribes(const QList<Subscribe>& subscribes);

    Subscribe getCurrentConfig();
    void setCurrentConfig(const Subscribe& subscribe);
    Subscribe getSubscribeByName(const QString &name);
    Subscribe delSubscribeByIndex(int index);

    QJsonObject getProxyGroupsRule(const QString& name);
    void setProxyGroupsRule(const QString& name, const QString& group, const QString& proxy);

    void setStartAtLogin(bool autoStart);
    bool isStartAtLogin();

    void setAutoUpdate(bool autoUpdate);
    bool isAutoUpdate();

    void setSystemProxy(bool flag);
    bool isSystemProxy();

    QString getSecret();

    QDateTime getUpdateTime();

    QMap<QString, QString> diffConfigs();
    void setMode(const QString& mode);
    QString getMode();
    void setHttpPort(int port);
    int getHttpPort();
    void setSocksPort(int port);
    int getSocksPort();
    void setExternalControlPort(int port);
    int getExternalControlPort();
    void setAllowLan(bool flag);
    bool getAllowLan();
    void setLogLevel(const QString& level);
    QString getLogLevel();
};
