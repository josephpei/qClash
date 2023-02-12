#include <QPushButton>
#include <QButtonGroup>
#include <QGridLayout>
#include <QVBoxLayout>
#include "proxyGroupWidget.h"
#include "../core/clashApi.h"

ProxyGroupWidget::ProxyGroupWidget(ClashProxy* clashProxy, QString name, QWidget *parent)
: CollapseWidget(clashProxy->getProxyByName(name).name, 100, parent)
, clashProxy(clashProxy)
, name(name)
{
    auto proxyGroup = clashProxy->getProxyByName(name);
    setObjectName(proxyGroup.name);
    setHeaderLayout(proxyGroup);
    setupUI(proxyGroup);
}

void ProxyGroupWidget::setHeaderLayout(const Proxy& proxyGroup) {
    groupName = new QLabel(proxyGroup.name);
    selectProxy = new QLabel(proxyGroup.now);
    selectProxy->setStyleSheet("border: 1px solid gray; border-radius: 3px; margin: 4px");
    latencyTest = new QPushButton(tr("Latency"));
    latencyTest->setStyleSheet("border: 1px solid gray; border-radius: 3px; margin: 4px");
    connect(latencyTest, &QPushButton::clicked, this, &ProxyGroupWidget::testProxyLatency);
    headerLayout.addWidget(groupName);
    headerLayout.addWidget(selectProxy);
    headerLayout.addWidget(latencyTest);

    headerLayout.addStretch(1);
    headerLayout.addWidget(&toggleButton);
    mainLayout.addLayout(&headerLayout, 0, 0, 1, 3);
}

void ProxyGroupWidget::setupUI(const Proxy& proxyGroup) {
    auto proxiesLayout = new QGridLayout;
    auto buttonGroup = new QButtonGroup;
    int row, col;
    for (int i = 0; i < proxyGroup.all.size(); ++i) {
        row = i / 2;
        col = i % 2 == 0 ? 0 : 1;
        QString text = proxyGroup.all[i] + "\n" + clashProxy->getProxyByName(proxyGroup.all[i]).type;
        auto button = new QPushButton(text);
        button->setObjectName(proxyGroup.all[i]);
        buttonMap.insert(proxyGroup.all[i], button);
        buttonGroup->addButton(button);
        if (proxyGroup.all[i] == proxyGroup.now) {
            button->setStyleSheet("QPushButton {background: gray}");
            oldButton = button;
        }
        proxiesLayout->addWidget(button, row, col);
    }
    setContentLayout(*proxiesLayout);
    connect(buttonGroup, &QButtonGroup::idClicked, this, [=](int id) {
        oldButton->setStyleSheet("");
        auto btn = buttonGroup->button(id);
        oldButton = qobject_cast<QPushButton*>(btn);
        btn->setStyleSheet("QPushButton {background: gray}");
        emit buttonProxyGroup(title, btn->objectName());
    });
}

void ProxyGroupWidget::setProxy(const QString &proxyName) {
    selectProxy->setText(proxyName);
}
void ProxyGroupWidget::testProxyLatency()
{
    clashProxy->update(ClashApi::getProxies());
    for (auto iter = buttonMap.begin(); iter != buttonMap.end(); ++iter) {
        auto proxy = clashProxy->getProxyByName(iter.key());
        if (proxy.isProxy()) {
            QString text = QString("%1\n%2\t%3ms").arg(proxy.name, proxy.type, QString::number(proxy.delay));
            iter.value()->setText(text);
        }
    }
}
