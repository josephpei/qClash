#pragma once

#include <QDialog>
#include <QPointer>

#include "subscribeNewDialog.h"

QT_BEGIN_NAMESPACE
class QAction;
class QVBoxLayout;
class QHBoxLayout;
class QTableView;
class QDialogButtonBox;
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

private:
    QVBoxLayout *vLayout;
    QTableView *tableView;
    QGroupBox *btnGroupBox;
    QHBoxLayout *btnLayout;
    QPushButton *subNewBtn;
    QPushButton *subDelBtn;
    QPushButton *subUpdateBtn;

    QPointer<SubscribeNewDialog> subscribeNewDialog;
};