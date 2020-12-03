#include "common.h"


QString jsonToString(const QJsonObject &json)
{
    return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

QJsonObject stringToJson(const QString str)
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

    if (json.contains("updateTime"))
        updateTime = QDateTime::fromString(json["updateTime"].toString(), Qt::SystemLocaleDate);
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
    json["updateTime"] = updateTime.toString(Qt::SystemLocaleDate);
    return json;
}