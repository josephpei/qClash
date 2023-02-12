#include "proxy.h"


bool Proxy::isProxy()
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

QVector<Proxy> ClashProxy::getProxyGroups()
{
    QVector<Proxy> groups;
//        std::copy_if(proxies.begin(), proxies.end(), std::back_inserter(groups),
//            [&](const Proxy& p) { return isProxyGroup(p.type); });
    for (auto &p : proxies) {
        if (isProxyGroup(p.type)) {
            groups.push_back(p);
        }
    }

    if (proxiesMap.contains("GLOBAL")) {
        auto global = proxiesMap.value("GLOBAL").all;
        QHash<QString, int> proxiesSortMap;
        for (int i = 0; i < global.size(); ++i) {
            proxiesSortMap[global[i]] = i;
        }
        std::sort(groups.begin(), groups.end(),
                  [&](const Proxy& p1, const Proxy& p2) {
                      return proxiesSortMap.value(p1.name) < proxiesSortMap.value(p2.name);
                  });
    }

    return groups;
}

QString ClashProxy::updateGroupProxy(const QString &old, const QString &now) {
    auto p = proxiesMap.find(old);
    auto o = p->now;
    p->now = now;
    return o;
}

Proxy ClashProxy::getProxyByName(const QString& name) {
    return proxiesMap.value(name);
}
