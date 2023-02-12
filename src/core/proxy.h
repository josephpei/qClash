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
        auto history = proxy.value("history").toArray();
        if (!history.isEmpty()) {
            auto lst = history.takeAt(0).toObject();
            delay = lst.value("delay").toInt();
        }
    }
    
    bool isProxy();
};

class ClashProxy {
private:
    QVector<Proxy> proxies;
    QHash<QString, Proxy> proxiesMap;

private:
    bool isProxyGroup(const QString& type);
    void init(const QJsonObject& providerProxies);

public:
    ClashProxy() = default;
    explicit ClashProxy(const QJsonObject& providerProxies)
    {
            init(providerProxies);
    }

    Proxy getProxyByName(const QString& name);
    QString updateGroupProxy(const QString& old, const QString& now);

    // get sorted proxy groups
    QVector<Proxy> getProxyGroups();

    void update(const QJsonObject& providerProxies) {
        proxies.clear();
        proxiesMap.clear();
        init(providerProxies);
    }
};


#endif // QCLASH_PROXY_H
