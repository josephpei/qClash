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
                         uint timeout,
                         const QNetworkProxy *proxy)
{
    QTimer timer;
    timer.setSingleShot(true);

    QNetworkRequest request(url);
    // request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    // request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    if (operation == QNetworkAccessManager::CustomOperation)
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/json; charset=UTF-8"));
    else
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    if (!secret.isEmpty())
        request.setRawHeader(QByteArray("Authorization"), QString("Bearer %1").arg(secret).toUtf8());
    QNetworkProxy noproxy;
    noproxy.setType(QNetworkProxy::NoProxy);
    inner->manager->setProxy(noproxy);
    if (proxy != nullptr)
        inner->manager->setProxy(*proxy);

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
    // qDebug() << "Url: " << url << ", Response: " << data;
    return data;
}

QByteArray HttpUtil::get(const QUrl &url, uint timeout, const QNetworkProxy *proxy)
{
    return request(url, QNetworkAccessManager::GetOperation, QByteArray(), timeout, proxy);
}

QByteArray HttpUtil::post(const QUrl &url, const QVariantMap &params)
{
    // QByteArray body;
    // QMapIterator<QString, QString> i(params);
    // while (i.hasNext()) {
    //     i.next();
    //     body += QUrl::toPercentEncoding(i.key()) + '=' + QUrl::toPercentEncoding(i.value()) + '&';
    // }
    QJsonDocument json = QJsonDocument::fromVariant(params);
    return request(url, QNetworkAccessManager::PostOperation, json.toJson(QJsonDocument::Compact));
}

QByteArray HttpUtil::put(const QUrl &url, const QVariantMap &params)
{
    QJsonDocument json = QJsonDocument::fromVariant(params);
    return request(url, QNetworkAccessManager::PutOperation, json.toJson(QJsonDocument::Compact));
}

QByteArray HttpUtil::patch(const QUrl &url, const QVariantMap &params)
{
    QJsonDocument json = QJsonDocument::fromVariant(params);
    return request(url, QNetworkAccessManager::CustomOperation, json.toJson(QJsonDocument::Compact));
}