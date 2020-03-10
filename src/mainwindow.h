#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

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

private:
    Ui::MainWindow *ui;

private:
    void createActions();
    void createTrayIcon();

    // tray menus & actions
    QAction *quitAction;
    QAction *proxyGlobalMode;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QMenu *proxyModeMenu;
};
#endif // MAINWINDOW_H
