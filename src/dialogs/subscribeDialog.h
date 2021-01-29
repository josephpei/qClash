#pragma once

#include <QDialog>
#include <QPointer>

#include "subscribeNewDialog.h"
#include "../core/configurator.h"
#include "../core/common.h"

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

signals:
    void subscribesUpdated();
    void subscribeAdded(const Subscribe& subscribe);
    void subscribeDeleted(int index);

private slots:
    void showSubNewDlg();
    void addSubscribe(const Subscribe &newSubscribe);
    void delSubscribe();
    void updateSubscribes();
    void updateCell(const QModelIndex& indexA, const QModelIndex& indexB);

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