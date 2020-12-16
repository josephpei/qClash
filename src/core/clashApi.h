#pragma once

#include "configurator.h"
#include "../utils/httputil.h"

class ClashApi {
private:
    static Configurator &configurator;
    static HttpUtil &http;

public:
    static void setGroupProxy(const QString &group, const QString &proxy);
};
