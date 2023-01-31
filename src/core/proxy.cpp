#include "proxy.h"


bool ClashProxy::isProxy(const QString& type)
{
    if (type == "Shadowsocks" || type == "ShadowsocksR" || type == "Vmess" || type == "Trojan" || type == "Snell" || type == "Socks5" || type == "Http")
        return true;
    return false;
}

bool ClashProxy::isProxyGroup(const QString& type)
{
    if (type == "Selector" || type == "URLTest" || type == "Fallback" || type == "LoadBalance" || type == "Relay")
        return true;
    return false;
}

void ClashProxy::init(const QJsonObject& providerProxies)
{
    for (auto it = providerProxies.begin(); it != providerProxies.end(); ++it) {
        QString name = it.key();
        QJsonObject obj = it.value().toObject();
        Proxy p(obj);
        proxies.push_back(p);
        proxiesMap[name] = p;
    }
}
