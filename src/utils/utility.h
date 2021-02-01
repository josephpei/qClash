#pragma once

#include "httputil.h"

class Utility {
private:
    static HttpUtil &http;

private:
    static QList<int> getVersion(QString version);

public:
    static QString getLatestVersion();
    static bool isVersionNewer(const QString& currentVersion, const QString& version);

    static void downloadLatestCountryMMDB(const QNetworkProxy* proxy);

    static QString netSpeedStr(int n);

    static bool isPortUsed(int port);
};