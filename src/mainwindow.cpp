#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QAction>
#include <QMenu>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();

    // connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    trayIcon->show();
}

MainWindow::~MainWindow()
{
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

    proxyGlobalMode = new QAction(tr("Global Mode"), this);
}

void MainWindow::createTrayIcon()
{
    trayMenu = new QMenu(this);

    proxyModeMenu = new QMenu("Proxy Mode", this);
    proxyModeMenu->addAction(proxyGlobalMode);

    trayMenu->addMenu(proxyModeMenu);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/assets/icons/tray.svg"));
}
