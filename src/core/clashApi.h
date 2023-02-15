#pragma once

#include "configurator.h"
#include "../utils/httputil.h"

class ClashApi {
private:
    static Configurator &configurator;
    static HttpUtil &http;
    static QString baseUrl;

public:
    static void setSecret(const QString& secret);
    static void reloadConfigs(bool force = false, QString path = "");
    static QByteArray setGroupProxy(const QString& group, const QString& proxy);
    static QJsonObject getConfigs();
    static QJsonObject getProxies();
    static QByteArray changeConfigs(const QVariantMap& configs);
    static QByteArray setMode(const QString &mode);
    static QByteArray setHttpPort(int port);
    static QByteArray setSocksPort(int port);
    static QByteArray setAllowLan(bool flag);
    static QByteArray setProxyMode(const QString& mode);
    static QByteArray setLogLevel(const QString& level);
};
