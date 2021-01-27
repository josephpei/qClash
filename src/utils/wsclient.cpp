#include "wsclient.h"
#include <QDebug>

WsClient::WsClient(const QUrl& url, QObject* parent)
    : QObject(parent)
    , url(url)
{
    connect(&ws, &QWebSocket::connected, this, &WsClient::onConnected);
    connect(&ws, &QWebSocket::disconnected, this, &WsClient::onDisconnected);
    ws.open(url);
}

void WsClient::onConnected()
{
    connect(&ws, &QWebSocket::textMessageReceived, this, &WsClient::onTextMessageReceived);
}

void WsClient::onDisconnected()
{
    qDebug() << "ws disconnected";
    reconnect();
}

void WsClient::onTextMessageReceived(const QString& message)
{
    // qDebug() << "Message received: " << message;
    emit trafficReceived(message);
}

void WsClient::reconnect()
{
    ws.abort();
    ws.open(url);
}

void WsClient::reset(const QUrl& url)
{
    ws.abort();
    ws.open(url);
}