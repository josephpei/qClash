#pragma once

#include "httputil.h"

class Utility {
private:
    static HttpUtil &http;

private:
    static QList<int> getVersion(QString version);

public:
    static const QString getLatestVersion();
    static bool isVersionNewer(const QString& currentVersion, const QString& version);
};