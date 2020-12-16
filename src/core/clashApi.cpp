#include "clashApi.h"

#include <QDebug>

Configurator &ClashApi::configurator = Configurator::instance();
HttpUtil &ClashApi::http = HttpUtil::instance();

void ClashApi::setGroupProxy(const QString &group, const QString &proxy)
{
    QUrl url("http://127.0.0.1:9090/proxies/" + group);
    QMap<QString, QString> params;
    params["name"] = proxy;
    qDebug() << proxy;

    QByteArray resp = http.put(url, params);
    qDebug() << QString(resp);
}