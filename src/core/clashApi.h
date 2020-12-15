#pragma once

#include "configurator.h"
#include "../utils/httputil.h"

Configurator &configurator = Configurator::instance();
HttpUtil &http = HttpUtil::instance();

void setGroupProxy(const QString &group, const QString &proxy);