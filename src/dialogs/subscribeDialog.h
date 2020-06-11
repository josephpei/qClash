#pragma once

#include <QDialog>
#include <QPointer>

#include "subscribeNewDialog.h"
#include "../core/configurator.h"

QT_BEGIN_NAMESPACE
class QAction;
class QVBoxLayout;
class QHBoxLayout;
class QTableView;
class QFrame;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class SubscribeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubscribeDialog(QWidget *parent = nullptr);

private slots:
    void showSubNewDlg();
    void addSubscribe(const QStringList &newSubscribe);
    void delSubscribe();
    void updateCell(const QModelIndex & indexA, const QModelIndex & indexB);

private:
    Configurator& configurator;

    QVBoxLayout *vLayout;
    QTableView *tableView;
    QFrame *btnFrame;
    QHBoxLayout *btnLayout;
    QPushButton *subNewBtn;
    QPushButton *subDelBtn;
    QPushButton *subUpdateBtn;

    QPointer<SubscribeNewDialog> subscribeNewDialog;
};