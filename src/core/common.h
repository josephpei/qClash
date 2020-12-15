#pragma once
#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMetaType>
    
enum ProxyType
{
    SS,
    SSR,
    VMESS,
    TROJAN
};

QString jsonToString(const QJsonObject &json);
QJsonObject stringToJson(const QString str);

struct Subscribe {
    QString name;
    QString url;
    QDateTime updateTime;
    bool updating;

    Subscribe() { }
    Subscribe(QString name, QString url = "") : name(name), url(url), updating(false) { }
    explicit Subscribe(const QJsonObject &json);

    Subscribe read(const QJsonObject &json);
    QJsonObject write() const;
};

Q_DECLARE_METATYPE(Subscribe);