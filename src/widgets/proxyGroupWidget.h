#ifndef QCLASH_PROXYGROUPWIDGET_H
#define QCLASH_PROXYGROUPWIDGET_H
#include <QLabel>
#include "collapseWidget.h"
#include "../core/proxy.h"

class ProxyGroupWidget: public CollapseWidget {
    Q_OBJECT

public:
    explicit ProxyGroupWidget(ClashProxy* clashProxy, QString name, QWidget* parent = nullptr);

public:
    void setHeaderLayout(const Proxy& proxyGroup);
    void setupUI(const Proxy& proxyGroup);
    void setProxy(const QString& proxyName);

signals:
    void buttonProxyGroup(const QString&, const QString&);
    
private slots:
    void testProxyLatency();

private:
    ClashProxy *clashProxy{ nullptr};
    QString name;
    QHash<QString, QPushButton*> buttonMap;
    
    QLabel* groupName{nullptr};
    QLabel* selectProxy{nullptr};
    QPushButton* latencyTest{nullptr};

    QPushButton* oldButton{nullptr};
};

#endif // QCLASH_PROXYGROUPWIDGET_H
