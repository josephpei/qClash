#pragma once

#include <QObject>
#include <QtWebSockets/QWebSocket>

class WsClient : public QObject {
    Q_OBJECT
public:
    explicit WsClient(const QUrl& url, QObject* parent = nullptr);

signals:
    void closed();

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);

    void reconnect();

private:
    QWebSocket ws;
    QUrl url;
};