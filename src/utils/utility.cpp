#include "utility.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

HttpUtil &Utility::http = HttpUtil::instance();

const QString Utility::getLatestVersion()
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
    for (QString v : version.split('.')) {
        _version.append(v.toInt());
    }
    return _version;
}
