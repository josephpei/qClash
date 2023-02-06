#ifndef QCLASH_PROXYGROUPWIDGET_H
#define QCLASH_PROXYGROUPWIDGET_H
#include "collapseWidget.h"
#include "../core/proxy.h"

class ProxyGroupWidget: public CollapseWidget {
    Q_OBJECT

public:
    explicit ProxyGroupWidget(const Proxy& proxyGroup, QWidget* parent = nullptr);

public:
    void setHeaderLayout(const Proxy& proxyGroup);
    void setupUI(const Proxy& proxyGroup);

};

#endif // QCLASH_PROXYGROUPWIDGET_H
