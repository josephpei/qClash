#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QGuiApplication>

#include "configurator.h"
#include "../utils/networkproxy.h"

Configurator &Configurator::instance()
{
    static Configurator configuratorInstance;
    return configuratorInstance;
}

const QString Configurator::getAppFilePath()
{
    if (QProcessEnvironment::systemEnvironment().contains("APPIMAGE")) {
        return QProcessEnvironment::systemEnvironment().value("APPIMAGE");
    }
    return QGuiApplication::applicationFilePath();
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
    QString tmpFile = "/tmp/" + name + ".yaml";
    QFile configFile(tmpFile);
    if (QFile::exists(tmpFile))
        QFile::remove(tmpFile);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // open file failed
        return;
    } else {
        configFile.write(content.toUtf8());
        if (QFile::exists(filePath))
            QFile::remove(filePath);
        QFile::copy(tmpFile, filePath);
    }
}

YAML::Node Configurator::loadClashConfig(const QString& name)
{
    QString configFile = Configurator::getClashConfigPath(name);
    try {
        root = YAML::LoadFile(configFile.toStdString());
    } catch (YAML::BadFile error) {
        throw error;
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

const QString Configurator::getSecret()
{
    if (root["secret"])
        return root["secret"].as<std::string>().c_str();
    return QString("");
}

QList<Subscribe> Configurator::getSubscribes()
{
    QList<QString> data = loadValue("subscribes").value<QList<QString>>();
    QList<Subscribe> subscribes;
    if (data.isEmpty())
        subscribes.append(Subscribe("config"));
    for (int i = 0; i < data.size(); i++) {
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

void Configurator::setStartAtLogin(bool autoStart)
{
    saveValue("startAtLogin", autoStart);
    const QString APP_NAME = "qClash";
    const QString APP_PATH = QDir::toNativeSeparators(getAppFilePath());
#if defined(Q_OS_WIN)
    QSettings win_settings(
        "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat);
#elif defined(Q_OS_LINUX)
    QFile srcFile(":/tpl-linux-autostart.desktop");
    QFile dstFile(QString("%1/.config/autostart/qClash.desktop")
                      .arg(QDir::homePath()));
#elif defined(Q_OS_MAC)
    QFile srcFile(":/tpl-macos-autostart.plist");
    QFile dstFile(
        QString("%1/Library/LaunchAgents/com.clash.desktop.launcher.plist")
            .arg(QDir::homePath()));
#endif

#if defined(Q_OS_WIN)
    if (autoStart) {
        win_settings.setValue(APP_NAME, APP_PATH);
    } else {
        win_settings.remove(APP_NAME);
    }
#elif defined(Q_OS_LINUX) or defined(Q_OS_MAC)
    if (autoStart) {
        QString fileContent;
        if (srcFile.exists() && srcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            fileContent = srcFile.readAll();
            srcFile.close();
        }
        QFileInfo dstFileInfo(dstFile);
        if (!dstFileInfo.dir().exists()) {
            dstFileInfo.dir().mkpath(".");
        }
        if (dstFile.open(QIODevice::WriteOnly)) {
            dstFile.write(fileContent.arg(APP_PATH).toUtf8());
            dstFile.close();
        }
    } else {
        if (dstFile.exists()) {
            dstFile.remove();
        }
    }
#endif
}

const bool Configurator::isStartAtLogin()
{
    return loadValue("startAtLogin", false).toBool();
}

void Configurator::setAutoUpdate(bool autoUpdate)
{
    saveValue("autoUpdate", autoUpdate);
}

const bool Configurator::isAutoUpdate()
{
    return loadValue("autoUpdate", true).toBool();
}

void Configurator::setSystemProxy(bool flag)
{
    saveValue("systemProxy", flag);
    if (flag) {
        NetworkProxy httpProxy("http", "127.0.0.1",
            getHttpPort(), NetworkProxyMode::GLOBAL_MODE);
        NetworkProxyHelper::setSystemProxy(httpProxy);
#ifndef Q_OS_WIN
        NetworkProxy socksProxy("socks", "127.0.0.1",
            getSocksPort(), NetworkProxyMode::GLOBAL_MODE);
        NetworkProxyHelper::setSystemProxy(socksProxy);
#endif
    } else {
        NetworkProxyHelper::resetSystemProxy();
    }
}

const bool Configurator::isSystemProxy()
{
    return loadValue("systemProxy", false).toBool();
}

QMap<QString, QString> Configurator::diffConfigs()
{
    QMap<QString, QString> configs;

    QString modeSaved = loadValue("mode", "").toString();
    QString modeYaml = root["mode"].as<std::string>().c_str();
    if (!modeSaved.isEmpty() && modeSaved != modeYaml)
        configs["mode"] = modeSaved;

    int httpPortSaved = loadValue("port", -1).toInt();
    int httpPortYaml = root["port"].as<int>();
    if (httpPortSaved != -1 && httpPortSaved != httpPortYaml)
        configs["port"] = QString::number(httpPortSaved);

    int socksPortSaved = loadValue("socksPort", -1).toInt();
    int socksPortYaml = root["socks-port"].as<int>();
    if (socksPortSaved != -1 && socksPortSaved != socksPortYaml)
        configs["socks-port"] = QString::number(socksPortSaved);

    bool allowLanSaved = loadValue("allowLan", false).toBool();
    bool allowLanYaml = root["allow-lan"].as<bool>();
    if (allowLanSaved != allowLanYaml)
        configs["allow-lan"] = QString::number(allowLanSaved);

    QString logLevelSaved = loadValue("logLevel", "").toString();
    QString logLevelYaml = root["log-level"].as<std::string>().c_str();
    if (!logLevelSaved.isEmpty() && logLevelSaved != logLevelYaml)
        configs["log-level"] = logLevelSaved;

    return configs;
}

void Configurator::setMode(const QString& mode)
{
    saveValue("mode", mode);
}

const QString Configurator::getMode()
{
    return loadValue("mode", root["mode"].as<std::string>().c_str()).toString();
}

void Configurator::setHttpPort(const int& port)
{
    saveValue("httpPort", port);
}

const int Configurator::getHttpPort()
{
    return loadValue("port", root["port"].as<int>()).toInt();
}

void Configurator::setSocksPort(const int& port)
{
    saveValue("socksPort", port);
}

const int Configurator::getSocksPort()
{
    return loadValue("socksPort", root["socks-port"].as<int>()).toInt();
}

void Configurator::setExternalControlPort(const int& port)
{
    saveValue("externalControlPort", port);
}

const int Configurator::getExternalControlPort()
{
    return loadValue("externalControlPort", QString(root["external-controller"].as<std::string>().c_str()).split(":")[1].toInt()).toInt();
}

void Configurator::setAllowLan(bool flag)
{
    saveValue("allowLan", flag);
}

const bool Configurator::getAllowLan()
{
    return loadValue("allowLan", root["allow-lan"].as<bool>()).toBool();
}

void Configurator::setLogLevel(const QString& level)
{
    saveValue("logLevel", level);
}

const QString Configurator::getLogLevel()
{
    QString level;
    if (!root["log-level"])
        level = "info";
    else
        level = QString(root["log-level"].as<std::string>().c_str());
    return loadValue("logLevel", level).toString();
}