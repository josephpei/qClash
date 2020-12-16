#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "./dialogs/aboutDialog.h"
#include "./dialogs/subscribeDialog.h"
#include "./core/clashCore.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QPointer>
#include <QList>
#include <QVector>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showAboutDialog();
    void showSubscribeDialog();
    void updateSubActions();

    void trayActivated();
    void configChange();
    void proxyChange(QAction *);

private:
    Ui::MainWindow *ui;

private:
    Configurator& configurator;

    void createActions();
    void createTrayIcon();
    QVector<QMenu*> createProxyMenus();

    // tray menus & actions
    QAction *quitAction;
    QAction *proxyGlobalMode;
    QAction *proxyRuleMode;
    QAction *proxyDirectMode;

    QAction *setAsSystemProxy;
    QAction *copyShellCommand;

    QAction *startAtLogin;
    QAction *allowLan;

    QAction *manageSubConfig;
    QAction *updateSubConfig;
    QAction *autoUpdateSubConfig;

    QAction *about;
    QAction *checkUpdate;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QMenu *proxyModeMenu;
    QMenu *subConfigMenu;
    QMenu *helpMenu;

    enum
    {
        MaxMenu = 9
    };
    QAction *subActions[MaxMenu];
    QMenu *proxyGroupMenus[MaxMenu];

    QPointer<AboutDialog> aboutDialog;
    QPointer<SubscribeDialog> subscribeDialog;

    ClashCore &clashCore;
};
#endif // MAINWINDOW_H
