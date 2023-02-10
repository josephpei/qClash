#ifndef QCLASH_PROXYGROUPWIDGET_H
#define QCLASH_PROXYGROUPWIDGET_H
#include <QLabel>
#include "collapseWidget.h"
#include "../core/proxy.h"

class ProxyGroupWidget: public CollapseWidget {
    Q_OBJECT

public:
    explicit ProxyGroupWidget(const Proxy& proxyGroup, QWidget* parent = nullptr);

public:
    void setHeaderLayout(const Proxy& proxyGroup);
    void setupUI(const Proxy& proxyGroup);
    void setProxy(const QString& proxyName);

signals:
    void buttonProxyGroup(const QString&, const QString&);

private:
    QLabel* groupName{nullptr};
    QLabel* selectProxy{nullptr};

    QPushButton* oldButton{nullptr};
};

#endif // QCLASH_PROXYGROUPWIDGET_H
