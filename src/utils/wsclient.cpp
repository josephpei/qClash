#include "wsclient.h"
#include <QDebug>

WsClient::WsClient(const QUrl& url, QObject* parent)
    : QObject(parent)
    , url(url)
{
    connect(&ws, &QWebSocket::connected, this, &WsClient::onConnected);
    connect(&ws, &QWebSocket::disconnected, this, &WsClient::closed);
    ws.open(QUrl(url));
}

void WsClient::onConnected()
{
    connect(&ws, &QWebSocket::textMessageReceived, this, &WsClient::onTextMessageReceived);
}

void WsClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received: " << message;
}

void WsClient::reconnect()
{
    ws.abort();
    ws.open(QUrl(url));
}