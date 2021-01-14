#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>

class HttpUtil : public QObject {
    Q_OBJECT
public:
    static HttpUtil &instance();

    QByteArray request(const QUrl &url,
                       QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation,
                       const QByteArray &body = QByteArray(),
                       uint timeout = 5000,
                       const QNetworkProxy* proxy = nullptr);
    QByteArray get(const QUrl &url, uint timeout = 5000, const QNetworkProxy *proxy = nullptr);
    QByteArray post(const QUrl &url, const QVariantMap &params);
    QByteArray put(const QUrl &url, const QVariantMap &params);
    QByteArray patch(const QUrl &url, const QVariantMap &params);

    void setSecret(const QString &);

signals:
    void finished(QNetworkReply *reply);

private:
    explicit HttpUtil(QObject *parent = nullptr);
    HttpUtil(const HttpUtil &) = delete;
    HttpUtil &operator=(HttpUtil rhs) = delete;

private:
    class Inner;
    friend class Inner;
    Inner *inner;
    QString secret;
};