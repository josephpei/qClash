#include "httputil.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QVariant>
#include <QBuffer>
#include <QTimer>
#include <QDebug>

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

void HttpUtil::setSecret(const QString& str)
{
    secret = str;
}

QByteArray HttpUtil::request(const QUrl &url,
                         QNetworkAccessManager::Operation operation,
                         const QByteArray &body,
                         uint timeout)
{
    QTimer timer;
    timer.setSingleShot(true);

    QNetworkRequest request(url);
    // request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    // request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    if (!secret.isEmpty())
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer %1").arg(secret).toUtf8());

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
    case QNetworkAccessManager::CustomOperation:
        reply = inner->manager->sendCustomRequest(request, "PATCH", body);
        break;
    }
    // sync
    // eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    // TODO: add error / timeout control
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    if (timeout > 0)
        timer.start(timeout);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Timeout handler
    if (timeout > 0 && !timer.isActive()) {
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        reply->abort();
        qWarning() << "Timeout when requesting " << url;
    }
    // Network error handler
    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Error occurred during requesting " << url
                    << "; Error: " << reply->error();
        return QByteArray();
    }

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
    // QByteArray body;
    QVariantMap vmap;
    QMapIterator<QString, QString> i(params);
    while (i.hasNext()) {
        i.next();
        // body += QUrl::toPercentEncoding(i.key()) + '=' + QUrl::toPercentEncoding(i.value()) + '&';
        vmap.insert(i.key(), i.value());
    }
    QJsonDocument json = QJsonDocument::fromVariant(vmap);
    return request(url, QNetworkAccessManager::PutOperation, json.toJson());
}

QByteArray HttpUtil::patch(const QUrl &url, const QMap<QString, QString> &params)
{
    QVariantMap vmap;
    QMapIterator<QString, QString> i(params);
    while (i.hasNext()) {
        i.next();
        // body += QUrl::toPercentEncoding(i.key()) + '=' + QUrl::toPercentEncoding(i.value()) + '&';
        vmap.insert(i.key(), i.value());
    }
    QJsonDocument json = QJsonDocument::fromVariant(vmap);
    return request(url, QNetworkAccessManager::CustomOperation, json.toJson());
}