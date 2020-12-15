#include "clashApi.h"


void setGroupProxy(const QString &group, const QString &proxy)
{
    QUrl url("http://127.0.0.1:9090/proxies/" + group);
    QMap<QString, QString> params;
    params["name"] = proxy;

    http.put(url, params);
}