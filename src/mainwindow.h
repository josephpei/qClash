#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "./dialogs/aboutDialog.h"
#include "./dialogs/subscribeDialog.h"
#include "./core/clashCore.h"
#include "./core/proxy.h"
#include "./utils/wsclient.h"
#include "./widgets/flowlayout.h"
#include "./widgets/collapseWidget.h"
#include "./widgets/proxyGroupWidget.h"

#include <QMainWindow>
#include <QPointer>
#include <QList>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QButtonGroup;
class QSystemTrayIcon;
class QJsonObject;
class QTimer;
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

signals:
    void mSubscribesUpdated(const QList<Subscribe>&);

private slots:
    void showMainWindow();

    void showAboutDialog();
    void checkLatestRelease();

    void showSubscribeDialog();
    void updateSubActions();
    void initConfigComboBox();
    void configComboBoxAdd(const Subscribe& subscribe);
    void configComboBoxDel(int index);

    void copyShellCommandClipboard();
    void updateSubscribes();

    void proxyGroupMenusChange();
    void modeChange(QAction *);
    void modeChange(int index);
    void configChange(QAction*);
    void configChange(int index);
    void proxyChange(QAction*);
    void startAtLoginChange(bool autoStart);
    void autoUpdateSubConfigChange(bool autoUpdate);
    void allowLanChange(bool flag);
    void systemProxyChange(bool flag);
    void logLevelChange(int index);
    void pageChange(int);
    void fillOverviewPage();
    void showNetTraffic(const QString& traffic);

    void downloadLastestCountryMMDB();

private:
    Ui::MainWindow *ui;

private:
    bool initClash();
    void setupMainWindow();
    void setupProxiesPage();
    void doConfigChange(const QString& name);

private:
    Configurator& configurator;
    QJsonObject proxies;
    bool configHasChanged;
    QVBoxLayout* proxiesLayout;
    //FlowLayout* proxiesLayout;

    QTimer* periodicTimer;

    void createActions();
    void createTrayIcon();

    // tray menus & actions
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
    QAction* downloadCountryMmdb;

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
    QButtonGroup* modeButtons;

    QPointer<AboutDialog> aboutDialog;
    QPointer<SubscribeDialog> subscribeDialog;

    ClashCore &clashCore;
    WsClient* wsClient;
    ClashProxy clashProxy;
};
#endif // MAINWINDOW_H
