#pragma once
#include <QObject>
#include <QString>
#include <QVariant>
#include <QSettings>
#include <yaml-cpp/yaml.h>

#include "common.h"


class Configurator : public QObject {
    Q_OBJECT

    Configurator() = default;

    QSettings config;

public:
    static Configurator& instance();

    static const QString getClashConfigPath();
    static const QString getClashConfigPath(const QString& name);
    static void saveClashConfig(const QString& name, const QString& content);
    static YAML::Node loadClashConfig(const QString& name);

    const QVariant loadValue(const QString& key, const QVariant& defaultValue = {});
    void saveValue(const QString& key, const QVariant& value);

    QList<Subscribe> getSubscribes();
    void setSubscribes(const QList<Subscribe>& subscribes);

    Subscribe getCurrentConfig();
    void setCurrentConfig(const Subscribe& subscribe);
    Subscribe getSubscribeByName(const QString &name);
};
