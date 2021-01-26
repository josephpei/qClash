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
    static void setGroupProxy(const QString& group, const QString& proxy);
    static QJsonObject getConfigs();
    static QJsonObject getProxies();
    static void changeConfigs(const QVariantMap& configs);
    static void setMode(const QString &mode);
    static void setHttpPort(int port);
    static void setSocksPort(int port);
    static void setAllowLan(bool flag);
    static void setProxyMode(const QString& mode);
    static void setLogLevel(const QString& level);
};
