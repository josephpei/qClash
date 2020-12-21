#include "clashApi.h"

#include <QDebug>

Configurator &ClashApi::configurator = Configurator::instance();
HttpUtil &ClashApi::http = HttpUtil::instance();
QString ClashApi::baseUrl = "http://127.0.0.1";

void ClashApi::setSecret(const QString& secret)
{
    http.setSecret(secret);
}

void ClashApi::setGroupProxy(const QString &group, const QString &proxy)
{
    QUrl url(QString("%1:%2/proxies/%3").arg(baseUrl).arg(configurator.getExternalControlPort()).arg(group));
    QMap<QString, QString> params;
    params["name"] = proxy;

    QByteArray resp = http.put(url, params);
    qDebug() << "Response: " << QString(resp);
}

QJsonObject ClashApi::getConfigs()
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QByteArray resp = http.get(url);
    return QJsonDocument::fromJson(resp).object();
}

void ClashApi::changeConfigs(const QMap<QString, QString> &configs)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QByteArray resp = http.patch(url, configs);
}

void ClashApi::setMode(const QString& mode)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QMap<QString, QString> params;
    params["mode"] = mode;
    QByteArray resp = http.patch(url, params);
    qDebug() << resp;
}

void ClashApi::setHttpPort(const int port)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QMap<QString, QString> params;
    params["port"] = port;

    QByteArray resp = http.patch(url, params);
    qDebug() << "Response: " << QString(resp);
}

void ClashApi::setSocksPort(const int port)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QMap<QString, QString> params;
    params["socks-port"] = port;

    QByteArray resp = http.patch(url, params);
}

void ClashApi::setAllowLan(const bool flag)
{
    QUrl url(QString("%1:%2/configs/").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QMap<QString, QString> params;
    params["allow-lan"] = flag ? "true" : "false";

    QByteArray resp = http.patch(url, params);
    qDebug() << resp;
}

void ClashApi::setProxyMode(const QString& mode)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QMap<QString, QString> params;
    params["mode"] = mode;

    QByteArray resp = http.patch(url, params);
    qDebug() << resp;
}

void ClashApi::setLogLevel(const QString& level)
{
    QUrl url(QString("%1:%2/configs").arg(baseUrl).arg(configurator.getExternalControlPort()));
    QMap<QString, QString> params;
    params["log-level"] = level;

    QByteArray resp = http.patch(url, params);
}