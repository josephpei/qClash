#pragma once

#include <QObject>
#include <QtWebSockets/QWebSocket>

class WsClient : public QObject {
    Q_OBJECT
public:
    explicit WsClient(const QUrl& url, QObject* parent = nullptr);

signals:
    void closed();
    void wsMessageReceived(const QString& message);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString& message);

    void reconnect();
    void reset(const QUrl& url);

private:
    QWebSocket ws;
    QUrl url;
};