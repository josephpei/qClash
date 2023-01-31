#pragma once
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QString>
#include <QDebug>


extern QString INT2LOGLEVEL[5];
extern QJsonObject LOGLEVEL2INT;
extern QString INT2PROXYMODE[3];
extern QJsonObject PROXYMODE2INT;

QString jsonToString(const QJsonObject& json);
QJsonObject stringToJson(const QString& str);

struct Subscribe {
    QString name;
    QString url;
    QDateTime updateTime;
    bool updating{};

    Subscribe() = default;
    explicit Subscribe(QString name, QString url = "", QDateTime updateTime = QDateTime::currentDateTime())
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