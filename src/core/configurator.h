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
    static YAML::Node loadClashConfig(QString name);

    const QVariant loadValue(const QString& key, const QVariant& defaultValue = {});
    void saveValue(const QString& key, const QVariant& value);

    QList<Subscribe> getSubscribes();
    void setSubscribes(const QList<Subscribe>& subscribes);

    Subscribe getCurrentSub();
    void setCurrentSub(const Subscribe &subscribe);
};
