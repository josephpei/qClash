#include "httputil.h"

#include <QEventLoop>

class HttpUtil::Inner
{
public:
    Inner(HttpUtil *h) : manager(new QNetworkAccessManager(h)) {}
    QNetworkAccessManager *manager;
};

HttpUtil::HttpUtil(QObject *parent) : QObject(parent)
{
    inner = new HttpUtil::Inner(this);
}

HttpUtil &HttpUtil::instance()
{
    static HttpUtil h;
    return h;
}

QByteArray HttpUtil::request(const QUrl &url,
                         QNetworkAccessManager::Operation operation,
                         const QByteArray &body,
                         uint offset)
{
    QNetworkRequest request(url);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    QNetworkReply *reply = nullptr;
    switch (operation)
    {
    case QNetworkAccessManager::GetOperation:
        reply = inner->manager->get(request);
        break;
    case QNetworkAccessManager::PostOperation:
        reply = inner->manager->post(request, body);
        break;
    case QNetworkAccessManager::PutOperation:
        reply = inner->manager->put(request, body);
        break;
    }
    // sync
    // eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    auto data = reply->readAll();
    reply->deleteLater();
    return data;
}

QByteArray HttpUtil::get(const QUrl &url)
{
    return request(url);
}

QByteArray HttpUtil::post(const QUrl &url, const QMap<QString, QString> &params)
{
    QByteArray body;
    QMapIterator<QString, QString> i(params);
    while (i.hasNext()) {
        i.next();
        body += QUrl::toPercentEncoding(i.key()) + '=' + QUrl::toPercentEncoding(i.value()) + '&';
    }
    return request(url, QNetworkAccessManager::PostOperation, body);
}

QByteArray HttpUtil::put(const QUrl &url, const QMap<QString, QString> &params)
{
    QByteArray body;
    QMapIterator<QString, QString> i(params);
    while (i.hasNext()) {
        i.next();
        body += QUrl::toPercentEncoding(i.key()) + '=' + QUrl::toPercentEncoding(i.value()) + '&';
    }
    return request(url, QNetworkAccessManager::PutOperation, body);
}