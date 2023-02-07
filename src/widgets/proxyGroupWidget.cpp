#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
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
    selectProxy->setStyleSheet("border: 1px solid gray; border-radius: 3px; margin: 4px");
    headerLayout.addWidget(groupName);
    headerLayout.addWidget(selectProxy);

    headerLayout.addStretch(1);
    headerLayout.addWidget(&toggleButton);
    mainLayout.addLayout(&headerLayout, 0, 0, 1, 3);
}

void ProxyGroupWidget::setupUI(const Proxy& proxyGroup) {
    auto proxiesLayout = new QGridLayout;
    int row, col;
    for (int i = 0; i < proxyGroup.all.size(); ++i) {
        row = i / 2;
        col = i % 2 == 0 ? 0 : 1;
        proxiesLayout->addWidget(new QPushButton(proxyGroup.all[i]), row, col);
    }
    setContentLayout(*proxiesLayout);
}
