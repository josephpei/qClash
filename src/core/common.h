#pragma once
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QString>
#include <QDebug>

enum ProxyType {
    SS,
    SSR,
    VMESS,
    TROJAN
};

extern QJsonObject LOGLEVEL2INT;

QString jsonToString(const QJsonObject& json);
QJsonObject stringToJson(const QString str);

struct Subscribe {
    QString name;
    QString url;
    QDateTime updateTime;
    bool updating;

    Subscribe() {}
    Subscribe(QString name, QString url = "", QDateTime updateTime = QDateTime::currentDateTime())
        : name(name)
        , url(url)
        , updateTime(updateTime)
        , updating(false)
    {
    }
    explicit Subscribe(const QJsonObject& json);

    Subscribe read(const QJsonObject& json);
    QJsonObject write() const;
};

Q_DECLARE_METATYPE(Subscribe);