#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include "proxyGroupWidget.h"

ProxyGroupWidget::ProxyGroupWidget(const Proxy &proxyGroup, QWidget *parent)
: CollapseWidget(proxyGroup.name, 100, parent)
{
    setObjectName(proxyGroup.name);
    setHeaderLayout(proxyGroup);
    setupUI(proxyGroup);
}

void ProxyGroupWidget::setHeaderLayout(const Proxy& proxyGroup) {
    groupName = new QLabel(proxyGroup.name);
    selectProxy = new QLabel(proxyGroup.now);
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
        auto button = new QPushButton(proxyGroup.all[i]);
        button->setObjectName(proxyGroup.all[i]);
        if (proxyGroup.all[i] == proxyGroup.now) {
            button->setStyleSheet("QPushButton {background: gray}");
        }
        proxiesLayout->addWidget(button, row, col);
    }
    setContentLayout(*proxiesLayout);
}

void ProxyGroupWidget::setProxy(const QString &proxyName) {
    selectProxy->setText(proxyName);
}
