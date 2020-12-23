#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./core/configurator.h"
#include "./core/clashApi.h"
#include "./utils/iconsForkAwesome.h"
#include <string>

#include <QAction>
#include <QActionGroup>
#include <QButtonGroup>
#include <QMenu>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QPixmap>
#include <QFontDatabase>
#include <QCloseEvent>
#include <QClipboard>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer)
    , pageButtons(new QButtonGroup)
    , clashCore(ClashCore::instance())
    , configurator(Configurator::instance())
{
    initClash();

    int fontId = QFontDatabase::addApplicationFont(":/forkawesome.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    qDebug() << "fontFamilies size: " << fontFamilies.size();
    QFont font;
    font.setFamily(fontFamilies.at(0));
    font.setPointSize(16);
    // font.setFamily("forkawesome");
    // font.setPixelSize(32);

    ui->setupUi(this);

    QPixmap logo(":/assets/icons/qClash.png");
    ui->logoLabel->setPixmap(logo);

    ui->overviewButton->setFont(font);
    QString overviewStr = QString("%1 %2").arg(QChar(0xf0e4)).arg(tr("Overview"));
    ui->overviewButton->setText(overviewStr);
    pageButtons->addButton(ui->overviewButton);
    ui->proxiesButton->setFont(font);
    QString proxiesStr = QString("%1 %2").arg(QChar(0xf1d8)).arg(tr("Proxies"));
    ui->proxiesButton->setText(proxiesStr);
    pageButtons->addButton(ui->proxiesButton);

    pageButtons->setExclusive(true);
    connect(pageButtons, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id) {
        ui->stackedWidget->setCurrentIndex(qAbs(id) - 2);
    });
    // connect(pageButtons, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(pageChange(QAbstractButton *)));

    createActions();
    createTrayIcon();

    fillOverviewPage();
    ui->overviewButton->setChecked(true);

    // connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);

    trayIcon->show();
    this->setWindowIcon(QIcon(":/assets/icons/qClash.png"));
}

MainWindow::~MainWindow()
{
    clashCore.stop();
    delete ui;
}

void MainWindow::showMainWindow()
{
    if (!isVisible())
        show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::initClash()
{
    Subscribe subscribe = configurator.getCurrentConfig();
    try {
        qDebug() << "Start with config: " << subscribe.name;
        configurator.loadClashConfig(subscribe.name);
    } catch (YAML::BadFile error) {
        QMessageBox::warning(this, "Config File Syntax Error", QString(error.msg.c_str()));
        subscribe = Subscribe("config");
        configurator.setCurrentConfig(subscribe);
        configurator.loadClashConfig(subscribe.name);
    } catch (YAML::BadFile error) {
        QMessageBox::warning(this, "Config File Syntax Error", QString(error.msg.c_str()));
        QApplication::quit();
    }
    QString configFilePath = configurator.getClashConfigPath(subscribe.name);
    clashCore.start(configFilePath);
    qDebug() << "Current configs: " << ClashApi::getConfigs();
    ClashApi::setSecret(configurator.getSecret());

    connect(timer, &QTimer::timeout, this, &MainWindow::updateSubscribes);
    timer->start(12*60*60*1000);
}

void MainWindow::createActions()
{
    mainWindowAction = new QAction(tr("Show MainWindow"), this);
    connect(mainWindowAction, &QAction::triggered, this, &MainWindow::showMainWindow);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QString modeSaved = configurator.getMode().toLower();
    modeActionsGroup = new QActionGroup(this);
    modeActionsGroup->setExclusive(true);
    proxyGlobalMode = new QAction(tr("Global"), this);
    proxyGlobalMode->setData("global");
    proxyGlobalMode->setCheckable(true);
    proxyRuleMode = new QAction(tr("Rule"), this);
    proxyRuleMode->setData("rule");
    proxyRuleMode->setCheckable(true);
    proxyDirectMode = new QAction(tr("Direct"), this);
    proxyDirectMode->setData("direct");
    proxyDirectMode->setCheckable(true);
    modeActionsGroup->addAction(proxyGlobalMode);
    modeActionsGroup->addAction(proxyRuleMode);
    modeActionsGroup->addAction(proxyDirectMode);
    if (modeSaved == "global")
        proxyGlobalMode->setChecked(true);
    else if (modeSaved == "rule")
        proxyRuleMode->setChecked(true);
    else if (modeSaved == "direct")
        proxyDirectMode->setChecked(true);
    connect(modeActionsGroup, SIGNAL(triggered(QAction *)), SLOT(modeChange(QAction *)));

    setAsSystemProxy = new QAction(tr("Set as system proxy"), this);
    setAsSystemProxy->setCheckable(true);
    setAsSystemProxy->setChecked(configurator.isSystemProxy());
    connect(setAsSystemProxy, &QAction::triggered, this, &MainWindow::systemProxyChange);

    copyShellCommand = new QAction(tr("Copy shell command"), this);
    connect(copyShellCommand, &QAction::triggered, this, &MainWindow::copyShellCommandClipboard);

    startAtLogin = new QAction(tr("Start at login"), this);
    startAtLogin->setCheckable(true);
    startAtLogin->setChecked(configurator.isStartAtLogin());
    connect(startAtLogin, &QAction::triggered, this, &MainWindow::startAtLoginChange);

    allowLan = new QAction(tr("Allow connect from lan"), this);
    allowLan->setCheckable(true);
    allowLan->setChecked(configurator.getAllowLan());
    connect(allowLan, &QAction::triggered, this, &MainWindow::allowLanChange);

    manageSubConfig = new QAction(tr("Manage"), this);
    connect(manageSubConfig, &QAction::triggered, this, &MainWindow::showSubscribeDialog);

    updateSubConfig = new QAction(tr("Update"), this);
    connect(updateSubConfig, &QAction::triggered, this, &MainWindow::updateSubscribes);

    autoUpdateSubConfig = new QAction(tr("Auto Update"), this);
    autoUpdateSubConfig->setCheckable(true);
    connect(autoUpdateSubConfig, &QAction::triggered, this, &MainWindow::autoUpdateSubConfigChange);

    about = new QAction(tr("About"), this);
    checkUpdate = new QAction(tr("Check Update"), this);
    connect(about, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

void MainWindow::proxyGroupMenusChange()
{
    Subscribe subscribe = configurator.getCurrentConfig();
    YAML::Node root = configurator.loadClashConfig(subscribe.name);
    QJsonObject proxyGroupsRule = configurator.getProxyGroupsRule(subscribe.name);
    int i = 0;
    for (YAML::const_iterator iter = root["proxy-groups"].begin(); iter != root["proxy-groups"].end(); ++iter)
    {
        QString groupName = (*iter)["name"].as<std::string>().c_str();
        QString selectProxy;
        if (proxyGroupsRule.contains(groupName)) {
            selectProxy = proxyGroupsRule[groupName].toString();
        }
        proxyGroupMenus[i]->setTitle(groupName);
        proxyGroupMenus[i]->menuAction()->setVisible(true);
        proxyGroupMenus[i]->clear();
        QActionGroup *actions = new QActionGroup(proxyGroupMenus[i]);
        for (YAML::const_iterator pi = (*iter)["proxies"].begin(); pi != (*iter)["proxies"].end(); ++pi)
        {
            QString proxyName = (*pi).as<std::string>().c_str();
            // QAction *proxyAction = new QAction(proxyName, groupMenu);
            QAction *action = proxyGroupMenus[i]->addAction(proxyName);
            action->setCheckable(true);
            if (proxyName == selectProxy) {
                action->setChecked(true);
                ClashApi::setGroupProxy(groupName, proxyName);
            }
            actions->addAction(action)->setData(proxyName);
        }
        connect(actions, SIGNAL(triggered(QAction *)), SLOT(proxyChange(QAction *)));
        ++i;
    }

    for (; i < MaxMenu; ++i) {
        proxyGroupMenus[i]->menuAction()->setVisible(false);
    }
}

void MainWindow::createTrayIcon()
{
    trayMenu = new QMenu(this);

    proxyModeMenu = new QMenu(tr("Proxy Mode"), this);
    proxyModeMenu->addAction(proxyGlobalMode);
    proxyModeMenu->addAction(proxyRuleMode);
    proxyModeMenu->addAction(proxyDirectMode);
    trayMenu->addMenu(proxyModeMenu);
    trayMenu->addSeparator();

    for (int i = 0; i < MaxMenu; i++) {
        proxyGroupMenus[i] = trayMenu->addMenu(QString());
        proxyGroupMenus[i]->setStyleSheet("* { menu-scrollable: 1 }");
        proxyGroupMenus[i]->menuAction()->setVisible(false);
    }
    proxyGroupMenusChange();
    trayMenu->addSeparator();

    trayMenu->addAction(setAsSystemProxy);
    trayMenu->addAction(copyShellCommand);
    trayMenu->addSeparator();

    trayMenu->addAction(startAtLogin);
    trayMenu->addAction(allowLan);
    trayMenu->addSeparator();

    subConfigMenu = new QMenu(tr("Config"), this);
    connect(subConfigMenu, &QMenu::aboutToShow, this, &MainWindow::updateSubActions);
    subActionsGroup = new QActionGroup(this);
    // subscribe action placeholer
    for (int i = 0; i < MaxMenu; ++i)
    {
        subActions[i] = subConfigMenu->addAction(QString());
        subActionsGroup->addAction(subActions[i]);
        subActions[i]->setCheckable(true);
        subActions[i]->setVisible(false);
    }
    connect(subActionsGroup, SIGNAL(triggered(QAction *)), SLOT(configChange(QAction *)));
    // default config
    subActions[0]->setText("config");
    subActions[0]->setData("config");
    subActions[0]->setVisible(true);

    subConfigMenu->addSeparator();
    subConfigMenu->addAction(manageSubConfig);
    subConfigMenu->addAction(updateSubConfig);
    subConfigMenu->addAction(autoUpdateSubConfig);
    trayMenu->addMenu(subConfigMenu);
    trayMenu->addSeparator();

    helpMenu = new QMenu(tr("Help"), this);
    helpMenu->addAction(about);
    helpMenu->addAction(checkUpdate);
    trayMenu->addMenu(helpMenu);
    trayMenu->addSeparator();

    trayMenu->addAction(mainWindowAction);
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/assets/icons/qClash.svg"));
}

void MainWindow::updateSubActions()
{
    QList<Subscribe> subs = configurator.getSubscribes();
    Subscribe currSub = configurator.getCurrentConfig();
    const int count = qMin(subs.size(), int(MaxMenu));
    int i = 0;
    for (; i < count; ++i)
    {
        // qDebug() << subs[i].name;
        subActions[i]->setText(subs[i].name);
        subActions[i]->setData(subs[i].name);
        subActions[i]->setVisible(true);
        if (subs[i].name == currSub.name)
            subActions[i]->setChecked(true);
    }
    for (; i < int(MaxMenu) - 1; ++i)
        subActions[i + 1]->setVisible(false);
}

void MainWindow::systemProxyChange(bool flag)
{
    configurator.setSystemProxy(flag);
}

void MainWindow::copyShellCommandClipboard()
{
    QString command = QString("export http_proxy=http://127.0.0.1:%1 https_proxy=http://127.0.0.1:%1")
                          .arg(configurator.getHttpPort());
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(command);
}

void MainWindow::updateSubscribes()
{
    QList<Subscribe> subscribes = configurator.getSubscribes();
    Subscribe currSub = configurator.getCurrentConfig();

    HttpUtil& http = HttpUtil::instance();
    QDateTime currTime = QDateTime::currentDateTime();
    for (int i = 0; i < subscribes.size(); ++i) {
        if (!subscribes[i].updating && !subscribes[i].url.isEmpty()) {
            qDebug() << "Update: " << subscribes[i].name;
            subscribes[i].updateTime = currTime;
            QByteArray data = http.get(subscribes[i].url);
            Configurator::saveClashConfig(subscribes[i].name, QString(data));
            if (currSub.name == subscribes[i].name) {
                ClashApi::reloadConfigs();
                proxyGroupMenusChange();
            }
        }
    }
    configurator.setSubscribes(subscribes);
}

void MainWindow::startAtLoginChange(bool autoStart)
{
    configurator.setStartAtLogin(autoStart);
}

void MainWindow::autoUpdateSubConfigChange(bool autoUpdate)
{
    if (autoUpdate) {

    } else {
        if (timer->isActive())
            timer->stop();
    }
}

void MainWindow::allowLanChange(bool flag)
{
    configurator.setAllowLan(flag);
    ClashApi::setAllowLan(flag);
}

void MainWindow::configChange(QAction *action)
{
    // if (const QAction *action = qobject_cast<const QAction *>(sender())) {
    QString name = action->data().toString();
    qDebug() << "Current config: " << name;
    configurator.setCurrentConfig(configurator.getSubscribeByName(name));
    QString configFilePath = configurator.getClashConfigPath(name);
    clashCore.restart(configFilePath);
    proxyGroupMenusChange();
    fillOverviewPage();
    // }
}

void MainWindow::modeChange(QAction *action)
{
    QString mode = action->data().toString();
    configurator.setMode(mode);
    ClashApi::setMode(mode);
}

void MainWindow::proxyChange(QAction *action)
{
    QString proxyName = action->data().toString();
    qDebug() << "Current proxy: " << proxyName;
    QWidget *widget = action->parentWidget();
    if (widget) {
        QMenu *menu = qobject_cast<QMenu *>(widget);
        qDebug() << "Current group: " << menu->title();
        ClashApi::setGroupProxy(menu->title(), proxyName);
        configurator.setProxyGroupsRule(configurator.getCurrentConfig().name, menu->title(), proxyName);
    }
}

void MainWindow::pageChange(int id)
{
    ui->stackedWidget->setCurrentIndex(id);
    // QString page = button->text();
    // qDebug() << "Current page: " << page;
    // if (page.contains("Overview")) {
    //     ui->stackedWidget->setCurrentIndex(0);
    // } else if (page.contains("Proxies")) {
    //     ui->stackedWidget->setCurrentIndex(1);
    // }
}

void MainWindow::fillOverviewPage()
{
    ui->httpPortLineEdit->setText(QString::number(configurator.getHttpPort()));
    ui->socksPortLineEdit->setText(QString::number(configurator.getSocksPort()));
    ui->exCtrlPortLineEdit->setText(QString::number(configurator.getExternalControlPort()));
    ui->allowLanCheckBox->setChecked(configurator.getAllowLan());
    ui->logLevelComboBox->setCurrentIndex(LOGLEVEL2INT[configurator.getLogLevel()].toInt());
}

void MainWindow::showAboutDialog()
{
    if (aboutDialog)
        aboutDialog->show();
    else {
        aboutDialog = new AboutDialog(this);
        aboutDialog->show();
    }
}

void MainWindow::showSubscribeDialog()
{
    if (subscribeDialog)
        subscribeDialog->exec();
    else {
        subscribeDialog = new SubscribeDialog(this);
        connect(subscribeDialog, SIGNAL(subscribesUpdated()), SLOT(proxyGroupMenusChange()));
        subscribeDialog->exec();
    }
}
