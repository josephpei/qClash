#include "common.h"

QString INT2LOGLEVEL[5] {
    "info",
    "warning",
    "error",
    "debug",
    "silent"
};

QJsonObject LOGLEVEL2INT{
    {"info", 0},
    {"warning", 1},
    {"error", 2},
    {"debug", 3},
    {"silent", 4}
};

QString INT2PROXYMODE[3] {
    "global",
    "rule",
    "direct"
};

QJsonObject PROXYMODE2INT {
    {"global", 0},
    {"rule", 1},
    {"direct", 2}
};

QString jsonToString(const QJsonObject &json)
{
    return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

QJsonObject stringToJson(const QString& str)
{
    QJsonObject json;

    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if (doc.isObject()) {
        json = doc.object();
    }
    return json;
}

Subscribe::Subscribe(const QJsonObject &json)
{
    if (json.contains("name"))
        name = json["name"].toString();

    if (json.contains("url"))
        url = json["url"].toString();

    if (json.contains("updateTime")) {
        updateTime = QLocale::system().toDateTime(json["updateTime"].toString(), QLocale::ShortFormat);
        // updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::SystemLocaleDate);
    }
}

Subscribe Subscribe::read(const QJsonObject &json)
{
    return Subscribe(json);
}

QJsonObject Subscribe::write() const
{
    QJsonObject json;
    json["name"] = name;
    json["url"] = url;
    json["updateTime"] = QLocale::system().toString(updateTime, QLocale::ShortFormat);
    //json["updateTime"] = updateTime.toString(Qt::SystemLocaleDate);
    return json;
}
