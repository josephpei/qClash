#include <QtWidgets>
#include <QDebug>

#include "subscribeDialog.h"

SubscribeDialog::SubscribeDialog(QWidget *parent) : QDialog(parent),
    configurator(Configurator::instance())
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

    // tableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->horizontalHeader()->setStretchLastSection(true);
    vLayout->addWidget(tableView);
    vLayout->addWidget(btnGroupBox);
    setLayout(vLayout);

    connect(subNewBtn, &QPushButton::clicked, this, &SubscribeDialog::showSubNewDlg);
}

void SubscribeDialog::showSubNewDlg()
{
    if (!subscribeNewDialog) {
        subscribeNewDialog = new SubscribeNewDialog;

        QStandardItemModel *model = new QStandardItemModel;
        this->tableView->setModel(model);
        model->setColumnCount(2);
        model->setHeaderData(0, Qt::Horizontal, tr("Subscribe Name"));
        model->setHeaderData(1, Qt::Horizontal, tr("Subscribe Url"));
        // this->tableView->setColumnWidth(1, 255);

        QList<QStringList> subscribes = configurator.getSubscribes();
        for (int i = 0; i < subscribes.count(); i++) {
            model->setItem(i, 0, new QStandardItem(subscribes[i][0]));
            model->setItem(i, 1, new QStandardItem(subscribes[i][1]));
        }
        this->tableView->show();

        connect(subscribeNewDialog.data(), &SubscribeNewDialog::newSubscribe, this, &SubscribeDialog::addSubscribe);
    }
}

void SubscribeDialog::addSubscribe(const QStringList &newSubscribe)
{
    QList<QStringList> subscribes = configurator.getSubscribes();
    int count = subscribes.count();
    subscribes.append(newSubscribe);
    configurator.setSubscribes(subscribes);

    QString subName = newSubscribe.at(0);
    QString subUrl = newSubscribe.at(1);
    // QMessageBox::information(this, "Add Subscribe", subName + subUrl);

    QStandardItemModel *model = (QStandardItemModel*)this->tableView->model();
    model->setItem(count, 0, new QStandardItem(subName));
    model->setItem(count, 1, new QStandardItem(subUrl));
    this->tableView->update();
}

