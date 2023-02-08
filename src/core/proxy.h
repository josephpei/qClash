#ifndef QCLASH_PROXY_H
#define QCLASH_PROXY_H
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QVector>
#include <algorithm>

struct Proxy {
    QString name;
    QString type;
    int delay {};
    bool udp {};

    QVector<QString> all;
    QString now;

    Proxy() = default;

    explicit Proxy(const QJsonObject& proxy)
    {
        auto pName = proxy.value("name");
        auto pType = proxy.value("type");
        auto pUdp = proxy.value("udp");
        auto pNow = proxy.value("now");
        auto pAll = proxy.value("all");
        // auto pHistory = proxy.value("history");

        name = pName.toString();
        type = pType.toString();
        udp = pUdp.isNull() && pUdp.toBool();
        now = pNow.isNull() ? "" : pNow.toString();
        if (!pAll.isNull()) {
            auto p = pAll.toArray();
            for (auto it = p.begin(); it != p.end(); ++it) {
                all.push_back(it->toString());
            }
        }
    }
};

class ClashProxy {
private:
    QVector<Proxy> proxies;
    QHash<QString, Proxy> proxiesMap;

private:
    bool isProxy(const QString& type);
    bool isProxyGroup(const QString& type);
    void init(const QJsonObject& providerProxies);

public:
    ClashProxy() = default;
    explicit ClashProxy(const QJsonObject& providerProxies)
    {
            init(providerProxies);
    }

    Proxy getProxyByName(QString name);

    // get sorted proxy groups
    QVector<Proxy> getProxyGroups()
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
    
    void update(const QJsonObject& providerProxies) {
        proxies.clear();
        proxiesMap.clear();
        init(providerProxies);
    }
};


#endif // QCLASH_PROXY_H
