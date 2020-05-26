#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./core/configurator.h"
#include "./utils/iconsForkAwesome.h"
#include <string>

#include <QAction>
#include <QMenu>
#include <QPixmap>
#include <QFontDatabase>
#include <QCloseEvent>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , clashCore(ClashCore::instance())
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
    ui->proxiesButton->setFont(font);
    QString proxiesStr = QString("%1 %2").arg(QChar(0xf1d8)).arg(tr("Proxies"));
    ui->proxiesButton->setText(proxiesStr);

    createActions();
    createTrayIcon();

    // connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    trayIcon->show();
    this->setWindowIcon(QIcon(":/assets/icons/icon.png"));
    clashCore.start();
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

    manageRemoteConfig = new QAction(tr("Manage"), this);
    updateRemoteConfig = new QAction(tr("Update"), this);
    autoUpdateRemoteConfig = new QAction(tr("Auto Update"), this);
    connect(manageRemoteConfig, &QAction::triggered, this, &MainWindow::showSubscribeDialog);

    about = new QAction(tr("About"), this);
    checkUpdate = new QAction(tr("Check Update"), this);
    connect(about, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

QVector<QMenu*> MainWindow::createProxyMenus()
{
    QVector<QMenu *> menus;
    YAML::Node root = Configurator::loadClashConfig();
    for (YAML::const_iterator iter = root["Proxy Group"].begin(); iter != root["Proxy Group"].end(); ++iter) {
        QString groupName = (*iter)["name"].as<std::string>().c_str();
        QMenu *groupMenu = new QMenu(groupName, this);
        for (YAML::const_iterator pi = (*iter)["proxies"].begin(); pi != (*iter)["proxies"].end(); ++pi) {
            QString proxyName = (*pi).as<std::string>().c_str();
            QAction *proxyAction = new QAction(proxyName, this);
            groupMenu->addAction(proxyAction);
        }
        menus.push_back(groupMenu);
    }
    return menus;
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

    QVector<QMenu *> menus = createProxyMenus();
    for (int i = 0; i < menus.size(); i++) {
        trayMenu->addMenu(menus.at(i));
    }
    trayMenu->addSeparator();

    trayMenu->addAction(setAsSystemProxy);
    trayMenu->addAction(copyShellCommand);
    trayMenu->addSeparator();

    trayMenu->addAction(startAtLogin);
    trayMenu->addAction(allowLan);
    trayMenu->addSeparator();

    remoteConfigMenu = new QMenu(tr("Config"), this);
    remoteConfigMenu->addAction(manageRemoteConfig);
    remoteConfigMenu->addAction(updateRemoteConfig);
    remoteConfigMenu->addAction(autoUpdateRemoteConfig);
    trayMenu->addMenu(remoteConfigMenu);
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
