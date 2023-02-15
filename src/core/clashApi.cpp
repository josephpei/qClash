#include "clashApi.h"

#include <QDebug>

Configurator &ClashApi::configurator = Configurator::instance();
HttpUtil &ClashApi::http = HttpUtil::instance();
QString ClashApi::baseUrl = "http://127.0.0.1";

void ClashApi::setSecret(const QString& secret)
{
    http.setSecret(secret);
}

void ClashApi::reloadConfigs(bool force, QString path)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    if (force)
        params["force"] = force;
    if (!path.isEmpty())
        params["path"] = path;
    http.put(url, params);
}

QByteArray ClashApi::setGroupProxy(const QString &group, const QString &proxy)
{
    QUrl url(QString("%1:%2/proxies/%3").arg(baseUrl).arg(configurator.getExternalControlPort()).arg(group));
    QVariantMap params;
    params["name"] = proxy;

    QByteArray resp = http.put(url, params);
    return resp;
}

QJsonObject ClashApi::getConfigs()
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QByteArray resp = http.get(url);
    return QJsonDocument::fromJson(resp).object();
}

QJsonObject ClashApi::getProxies()
{
    QUrl url(QString("%1:%2/proxies").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QByteArray resp = http.get(url);
    return QJsonDocument::fromJson(resp).object()["proxies"].toObject();
}

QByteArray ClashApi::changeConfigs(const QVariantMap &configs)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QByteArray resp = http.patch(url, configs);
    return resp;
}

QByteArray ClashApi::setMode(const QString& mode)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    params["mode"] = mode;
    QByteArray resp = http.patch(url, params);
    return resp;
}

QByteArray ClashApi::setHttpPort(int port)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    params["port"] = port;

    QByteArray resp = http.patch(url, params);
    return resp;
}

QByteArray ClashApi::setSocksPort(int port)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    params["socks-port"] = port;

    QByteArray resp = http.patch(url, params);
    return resp;
}

QByteArray ClashApi::setAllowLan(bool flag)
{
    QUrl url(QString("%1:%2/configs/").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    params["allow-lan"] = flag;

    QByteArray resp = http.patch(url, params);
    return resp;
}

QByteArray ClashApi::setProxyMode(const QString& mode)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    params["mode"] = mode;

    QByteArray resp = http.patch(url, params);
    return resp;
}

QByteArray ClashApi::setLogLevel(const QString& level)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QVariantMap params;
    params["log-level"] = level;

    QByteArray resp = http.patch(url, params);
    return resp;
}
