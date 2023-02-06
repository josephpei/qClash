#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "proxyGroupWidget.h"

ProxyGroupWidget::ProxyGroupWidget(const Proxy &proxyGroup, QWidget *parent)
: CollapseWidget(proxyGroup.name, 100, parent)
{
    setHeaderLayout(proxyGroup);
    setupUI(proxyGroup);
}

void ProxyGroupWidget::setHeaderLayout(const Proxy& proxyGroup) {
    auto groupName = new QLabel(proxyGroup.name);
    auto selectProxy = new QLabel(proxyGroup.now);
    headerLayout.addWidget(groupName);
    headerLayout.addWidget(selectProxy);

    headerLayout.addWidget(&toggleButton);
}

void ProxyGroupWidget::setupUI(const Proxy& proxyGroup) {
    auto proxiesLayout = new QVBoxLayout;
    for (auto &p : proxyGroup.all) {
        proxiesLayout->addWidget(new QPushButton(p));
    }
    setContentLayout(*proxiesLayout);
}
