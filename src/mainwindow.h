#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "./dialogs/aboutDialog.h"
#include "./dialogs/subscribeDialog.h"
#include "./core/clashCore.h"

#include <QMainWindow>
#include <QPointer>
#include <QList>
#include <QVector>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QButtonGroup;
class QSystemTrayIcon;
class QJsonObject;
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
    void showMainWindow();

    void showAboutDialog();
    void showSubscribeDialog();
    void updateSubActions();

    void copyShellCommandClipboard();

    void proxyGroupMenusChange();
    void modeChange(QAction *);
    void configChange(QAction *);
    void proxyChange(QAction *);
    void startAtLoginChange(bool autoStart);
    void allowLanChange(bool flag);
    void systemProxyChange(bool flag);
    void pageChange(int);
    void fillOverviewPage();

private:
    Ui::MainWindow *ui;

private:
    void initClash();

private:
    Configurator& configurator;

    void createActions();
    void createTrayIcon();
    QVector<QMenu*> createProxyMenus();

    // tray menus & actions
    QAction *mainWindowAction;
    QAction *quitAction;

    QActionGroup *modeActionsGroup;
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
        MaxMenu = 99
    };
    QAction *subActions[MaxMenu];
    QActionGroup *subActionsGroup;
    QMenu *proxyGroupMenus[MaxMenu];

    QButtonGroup *pageButtons;

    QPointer<AboutDialog> aboutDialog;
    QPointer<SubscribeDialog> subscribeDialog;

    ClashCore &clashCore;
};
#endif // MAINWINDOW_H
