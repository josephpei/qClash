#include <QtWidgets>

#include "subscribeDialog.h"

SubscribeDialog::SubscribeDialog(QWidget *parent) : QDialog(parent)
{
    // ui
    vLayout = new QVBoxLayout;
    tableView = new QTableView;

    subNewBtn = new QPushButton(tr("New"));
    subDelBtn = new QPushButton(tr("Delete"));
    subUpdateBtn = new QPushButton(tr("Update"));
    btnLayout = new QHBoxLayout;
    btnGroupBox = new QGroupBox;
    btnLayout->addStretch();
    btnLayout->addWidget(subNewBtn);
    btnLayout->addWidget(subDelBtn);
    btnLayout->addWidget(subUpdateBtn);
    btnGroupBox->setLayout(btnLayout);

    vLayout->addWidget(tableView);
    vLayout->addWidget(btnGroupBox);
    setLayout(vLayout);

    connect(subNewBtn, &QPushButton::clicked, this, &SubscribeDialog::showSubNewDlg);
}

void SubscribeDialog::showSubNewDlg()
{
    if (!subscribeNewDialog) {
        subscribeNewDialog = new SubscribeNewDialog;
        connect(subscribeNewDialog.data(), &SubscribeNewDialog::newSubscribe, this, &SubscribeDialog::addSubscribe);
    }
}

void SubscribeDialog::addSubscribe(const QStringList &newSubscribe)
{
    QString subName = newSubscribe.at(0);
    QString subUrl = newSubscribe.at(1);
    // QMessageBox::information(this, "Add Subscribe", subName + subUrl);
    QStandardItemModel *model = new QStandardItemModel;
    this->tableView->setModel(model);
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, tr("Subscribe Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Subscribe Url"));
    this->tableView->setColumnWidth(1, 255);
    model->setItem(0, 0, new QStandardItem(subName));
    model->setItem(0, 1, new QStandardItem(subUrl));
}

