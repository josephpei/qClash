#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./core/configurator.h"
#include "./core/clashApi.h"
#include "./utils/iconsForkAwesome.h"
#include <string>

#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QPixmap>
#include <QFontDatabase>
#include <QCloseEvent>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , clashCore(ClashCore::instance())
    , configurator(Configurator::instance())
{
    int fontId = QFontDatabase::addApplicationFont(":/forkawesome.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    qDebug() << "fontFamilies size: " << fontFamilies.size();
    QFont font;
    font.setFamily(fontFamilies.at(0));
    font.setPointSize(16);
    // font.setFamily("forkawesome");
    // font.setPixelSize(32);

    ui->setupUi(this);

    QPixmap logo(":/assets/icons/icon.png");
    ui->logoLabel->setPixmap(logo);

    ui->overviewButton->setFont(font);
    QString overviewStr = QString("%1 %2").arg(QChar(0xf0e4)).arg(tr("Overview"));
    ui->overviewButton->setText(overviewStr);
    connect(ui->overviewButton, &QPushButton::clicked, this, &MainWindow::pageChange);
    ui->proxiesButton->setFont(font);
    QString proxiesStr = QString("%1 %2").arg(QChar(0xf1d8)).arg(tr("Proxies"));
    ui->proxiesButton->setText(proxiesStr);
    connect(ui->proxiesButton, &QPushButton::clicked, this, &MainWindow::pageChange);

    createActions();
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);

    trayIcon->show();
    this->setWindowIcon(QIcon(":/assets/icons/icon.png"));

    Subscribe subscribe = configurator.getCurrentConfig();
    qDebug() << "Start with config: " << subscribe.name;
    configurator.loadClashConfig(subscribe.name);
    fillOverviewPage();
    QString configFilePath = configurator.getClashConfigPath(subscribe.name);
    clashCore.start(configFilePath);
}

MainWindow::~MainWindow()
{
    clashCore.stop();
    delete ui;
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

void MainWindow::createActions()
{
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    proxyGlobalMode = new QAction(tr("Global"), this);
    proxyRuleMode = new QAction(tr("Rule"), this);
    proxyDirectMode = new QAction(tr("Direct"), this);

    setAsSystemProxy = new QAction(tr("Set as system proxy"), this);
    copyShellCommand = new QAction(tr("Copy shell command"), this);

    startAtLogin = new QAction(tr("Start at login"), this);
    allowLan = new QAction(tr("Allow connect from lan"), this);

    manageSubConfig = new QAction(tr("Manage"), this);
    updateSubConfig = new QAction(tr("Update"), this);
    autoUpdateSubConfig = new QAction(tr("Auto Update"), this);
    connect(manageSubConfig, &QAction::triggered, this, &MainWindow::showSubscribeDialog);

    about = new QAction(tr("About"), this);
    checkUpdate = new QAction(tr("Check Update"), this);
    connect(about, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

void MainWindow::trayActivated()
{
    QVector<QMenu *> menus;
    YAML::Node root = configurator.loadClashConfig(configurator.getCurrentConfig().name);
    int i = 0;
    for (YAML::const_iterator iter = root["proxy-groups"].begin(); iter != root["proxy-groups"].end(); ++iter)
    {
        QString groupName = (*iter)["name"].as<std::string>().c_str();
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
    trayMenu->addSeparator();

    trayMenu->addAction(setAsSystemProxy);
    trayMenu->addAction(copyShellCommand);
    trayMenu->addSeparator();

    trayMenu->addAction(startAtLogin);
    trayMenu->addAction(allowLan);
    trayMenu->addSeparator();

    subConfigMenu = new QMenu(tr("Config"), this);
    connect(subConfigMenu, &QMenu::aboutToShow, this, &MainWindow::updateSubActions);
    // subscribe action placeholer
    for (int i = 0; i < MaxMenu; ++i)
    {
        subActions[i] = subConfigMenu->addAction(QString(), this, &MainWindow::configChange);
        subActions[i]->setCheckable(true);
        subActions[i]->setVisible(false);
    }
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

    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/assets/icons/icon.svg"));
}

void MainWindow::updateSubActions()
{
    QList<Subscribe> subs = configurator.getSubscribes();
    const int count = qMin(subs.size(), int(MaxMenu));
    int i = 0;
    for (; i < count; ++i)
    {
        qDebug() << subs[i].name;
        subActions[i + 1]->setText(subs[i].name);
        subActions[i + 1]->setData(subs[i].name);
        subActions[i + 1]->setVisible(true);
    }
    for (; i < int(MaxMenu) - 1; ++i)
        subActions[i + 1]->setVisible(false);
}

void MainWindow::configChange()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        QString name = action->data().toString();
        qDebug() << "Current config: " << name;
        configurator.setCurrentConfig(configurator.getSubscribeByName(name));
        QString configFilePath = configurator.getClashConfigPath(name);
        clashCore.restart(configFilePath);
    }
}

void MainWindow::proxyChange(QAction *action)
{
    QString proxyName = action->data().toString();
    qDebug() << proxyName;
    QWidget *widget = action->parentWidget();
    if (widget) {
        QMenu *menu = qobject_cast<QMenu *>(widget);
        qDebug() << menu->title();
        ClashApi::setGroupProxy(menu->title(), proxyName);
    }
}

void MainWindow::pageChange()
{
    if (const QPushButton *button = qobject_cast<const QPushButton*>(sender())) {
        QString page = button->text();
        qDebug() << "Current page: " << page;
        if (page.contains("Overview")) {
            ui->stackedWidget->setCurrentIndex(0);
        } else if (page.contains("Proxies")) {
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
}

void MainWindow::fillOverviewPage()
{
    ui->httpPortLabel->setText(configurator.getHttpPort());
    ui->socksPortLabel->setText(configurator.getSocksPort());
    ui->exControlPortLabel->setText(configurator.getExternalControlPort());
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
        subscribeDialog->exec();
    }
}
