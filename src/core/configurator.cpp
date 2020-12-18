#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>

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

const QString Configurator::getClashConfigPath(const QString& name)
{
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return homePath + "/.config/clash/" + name + ".yaml";
}

void Configurator::saveClashConfig(const QString& name, const QString& content)
{
    QString filePath = Configurator::getClashConfigPath(name);
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // open file failed
        return;
    } else {
        configFile.write(content.toUtf8());
    }
}

YAML::Node Configurator::loadClashConfig(const QString& name)
{
    QString configFile = Configurator::getClashConfigPath(name);
    try {
        root = YAML::LoadFile(configFile.toStdString());
    } catch (YAML::BadFile err) {

    }
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

QList<Subscribe> Configurator::getSubscribes()
{
    QList<QString> data = loadValue("subscribes").value<QList<QString>>();
    QList<Subscribe> subscribes;
    subscribes.append(Subscribe("config"));
    for (int i = 0; i < data.size(); i++)
    {
        subscribes.append(Subscribe(stringToJson(data[i])));
    }
    return subscribes;
}

void Configurator::setSubscribes(const QList<Subscribe> &subscribes)
{
    QList<QString> data;
    for (int i = 0; i < subscribes.size(); ++i) {
        data.append(jsonToString(subscribes[i].write()));
    }
    saveValue("subscribes", QVariant::fromValue(data));
}

Subscribe Configurator::getCurrentConfig()
{
    QString data = loadValue("currentConfig").value<QString>();
    if (data.isEmpty()) {
        return Subscribe("config");
    }
    return Subscribe(stringToJson(data));
}

void Configurator::setCurrentConfig(const Subscribe& subscribe)
{
    saveValue("currentConfig", QVariant::fromValue(jsonToString(subscribe.write())));
}

Subscribe Configurator::getSubscribeByName(const QString& name)
{
    QList<Subscribe> subscribes = getSubscribes();
    for (int i = 0; i < subscribes.size(); ++i) {
        if (subscribes[i].name == name)
            return subscribes[i];
    }
    return Subscribe("config");
}

QJsonObject Configurator::getProxyGroupsRule(const QString& name)
{
    proxyGroups = QJsonDocument::fromJson(loadValue("proxyGroupsRule").toString().toUtf8()).object();
    QJsonObject current;
    if (!proxyGroups.isEmpty())
        current = proxyGroups.value(name).toObject();
    // clean rule, remove same items in default config
    // saveValue("proxyGroupsRule", QVariant(proxyGroups));
    return current;
}

void Configurator::setProxyGroupsRule(const QString& name, const QString& group, const QString& proxy)
{
    QJsonObject oldRule = QJsonDocument::fromJson(loadValue("proxyGroupsRule").toString().toUtf8()).object();
    if (oldRule.isEmpty())
        oldRule.insert(name, QJsonObject());
    QJsonValueRef ref = oldRule.find(name).value();
    QJsonObject current = ref.toObject();
    current.insert(group, proxy);
    oldRule[name] = current;
    saveValue("proxyGroupsRule", QVariant(QJsonDocument(oldRule).toJson()));
}

QString Configurator::getHttpPort()
{
    return root["port"].as<std::string>().c_str();
}

QString Configurator::getSocksPort()
{
    return root["socks-port"].as<std::string>().c_str();
}

QString Configurator::getExternalControlPort()
{
    return QString(root["external-controller"].as<std::string>().c_str()).split(":")[1];
}

bool Configurator::getAllowLan()
{
    return root["allow-lan"].as<bool>();
}

QString Configurator::getLogLevel()
{
    if (!root["log-level"])
        return "info";
    return QString(root["log-level"].as<std::string>().c_str());
}