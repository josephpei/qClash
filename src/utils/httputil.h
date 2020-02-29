#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class HttpUtil : public QObject {
    Q_OBJECT
public:
    static HttpUtil &instance();

    QByteArray request(const QUrl &url,
                       QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation,
                       const QByteArray &body = QByteArray(),
                       uint offset = 0);
    QByteArray get(const QUrl &url);
    QByteArray post(const QUrl &url, const QMap<QString, QString> &params);
    QByteArray put(const QUrl &url, const QMap<QString, QString> &params);
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
};