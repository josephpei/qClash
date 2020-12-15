#include <QtWidgets>
#include <QDebug>
#include <QFile>

#include "subscribeDialog.h"
#include "../utils/httputil.h"
#include "../core/configurator.h"

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
    btnFrame = new QFrame;
    btnLayout->addStretch();
    btnLayout->addWidget(subNewBtn);
    btnLayout->addWidget(subDelBtn);
    btnLayout->addWidget(subUpdateBtn);
    btnFrame->setLayout(btnLayout);

    // tableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setSectionsClickable(false);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    QStandardItemModel *model = new QStandardItemModel;
    tableView->setModel(model);
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Url"));
    // this->tableView->setColumnWidth(1, 255);

    QList<Subscribe> subscribes = configurator.getSubscribes();
    for (int i = 0; i < subscribes.count(); i++) {
        model->setItem(i, 0, new QStandardItem(subscribes[i].name));
        model->setItem(i, 1, new QStandardItem(subscribes[i].url));
    }

    vLayout->addWidget(tableView);
    vLayout->addWidget(btnFrame);
    setLayout(vLayout);

    connect(tableView->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(updateCell(QModelIndex, QModelIndex)));
    connect(subNewBtn, &QPushButton::clicked, this, &SubscribeDialog::showSubNewDlg);
    connect(subDelBtn, &QPushButton::clicked, this, &SubscribeDialog::delSubscribe);
}

void SubscribeDialog::showSubNewDlg()
{
    if (!subscribeNewDialog) {
        subscribeNewDialog = new SubscribeNewDialog;

        connect(subscribeNewDialog.data(), &SubscribeNewDialog::newSubscribe, this, &SubscribeDialog::addSubscribe);
    }
}

void SubscribeDialog::addSubscribe(const Subscribe &newSubscribe)
{
    QList<Subscribe> subscribes = configurator.getSubscribes();
    int count = subscribes.count();
    subscribes.append(newSubscribe);
    configurator.setSubscribes(subscribes);

    QString subName = newSubscribe.name;
    QString subUrl = newSubscribe.url;
    // QMessageBox::information(this, "Add Subscribe", subName + subUrl);
    HttpUtil &http = HttpUtil::instance();
    QByteArray data = http.get(subUrl);
    Configurator::saveClashConfig(subName, QString(data));

    QStandardItemModel *model = (QStandardItemModel*)this->tableView->model();
    model->setItem(count, 0, new QStandardItem(subName));
    model->setItem(count, 1, new QStandardItem(subUrl));
}

void SubscribeDialog::delSubscribe()
{
    QModelIndex index = this->tableView->currentIndex();
    if (!index.isValid())
        return;
    QStandardItemModel *model = (QStandardItemModel*)this->tableView->model();

    QList<Subscribe> subscribes = configurator.getSubscribes();
    subscribes.removeAt(index.row());
    configurator.setSubscribes(subscribes);

    model->removeRow(index.row());
}

void SubscribeDialog::updateCell(const QModelIndex & indexA, const QModelIndex & indexB)
{
    int col = indexA.column();
    int row = indexB.row();
    QList<Subscribe> subscribes = configurator.getSubscribes();
    QStandardItemModel *model = (QStandardItemModel*)this->tableView->model();
    QString str = model->data(model->index(row, col)).toString();
    if (col > 1)
        return;
    if (col == 0)
        subscribes[row].name = str;
    if (col == 1)
        subscribes[row].url = str;
    configurator.setSubscribes(subscribes);
}

