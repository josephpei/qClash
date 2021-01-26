#include "utility.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>

HttpUtil &Utility::http = HttpUtil::instance();

QString Utility::getLatestVersion()
{
    QString url("https://api.github.com/repos/josephpei/qClash/releases");
    QByteArray releaseJsonStr = http.get(url);
    if (releaseJsonStr.isEmpty())
        return QString();
    QJsonObject latestRelease;
    QJsonDocument releaseJsonDoc = QJsonDocument::fromJson(releaseJsonStr);
    QJsonArray releases = releaseJsonDoc.array();
    for (int i = 0; i < releases.size(); ++i) {
        QJsonObject release = releases[i].toObject();
        latestRelease = release;
        break;
    }
    return latestRelease.empty() ? "" : latestRelease["name"].toString();
}

bool Utility::isVersionNewer(const QString& currentVersion, const QString& version)
{
    QList<int> _currentVersion = getVersion(currentVersion);
    QList<int> _version = getVersion(version);

    for (int i = 0; i < _version.size() && i < _currentVersion.size(); ++i) {
        if (_version.at(i) > _currentVersion.at(i)) {
            return true;
        } else if (_version.at(i) < _currentVersion.at(i)) {
            return false;
        }
    }
    return false;
}

QList<int> Utility::getVersion(QString version)
{
    QList<int> _version;
    if (version.startsWith('v') || version.startsWith('v')) {
        version = version.mid(1);
    }
    for (const QString& v : version.split('.')) {
        _version.append(v.toInt());
    }
    return _version;
}

void Utility::downloadLatestCountryMMDB(const QNetworkProxy *proxy)
{
    // https://dev.maxmind.com/geoip/geoip2/geolite2/
    // https://github.com/alecthw/mmdb_china_ip_list
    QString url("https://raw.githubusercontent.com/alecthw/mmdb_china_ip_list/release/Country.mmdb");
    QByteArray data = http.get(url, 500000, proxy);
    if (data.isEmpty())
        return;
    QString filename = "/tmp/Country.mmdb";
    if (QFile::exists(filename))
        QFile::remove(filename);
    QFile mmdb(filename);
    if (!mmdb.open(QIODevice::WriteOnly)) {
        return;
    } else {
        mmdb.write(data);
        QString oldfile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/clash/Country.mmdb";
        if (QFile::exists(oldfile))
            QFile::remove(oldfile);
        QFile::copy(filename, oldfile);
    }
}