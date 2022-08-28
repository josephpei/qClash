#include <QtWidgets>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>

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

    auto *model = new QStandardItemModel;
    tableView->setModel(model);
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Url"));
    model->setHeaderData(2, Qt::Horizontal, tr("Time"));
    tableView->horizontalHeader()->resizeSection(1, 420);

    updateTable(configurator.getSubscribes());

    vLayout->addWidget(tableView);
    vLayout->addWidget(btnFrame);
    setLayout(vLayout);
    resize(700, 300);

    connect(tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(updateCell(QModelIndex,QModelIndex)));
    connect(subNewBtn, &QPushButton::clicked, this, &SubscribeDialog::showSubNewDlg);
    connect(subDelBtn, &QPushButton::clicked, this, &SubscribeDialog::delSubscribe);
    connect(subUpdateBtn, &QPushButton::clicked, this, &SubscribeDialog::updateSubscribes);
}

void SubscribeDialog::updateTable(const QList<Subscribe>& subscribes)
{
    // QList<Subscribe> subscribes = configurator.getSubscribes();
    auto* model = (QStandardItemModel*)this->tableView->model();
    for (int i = 1; i < subscribes.count(); i++) {
        model->setItem(i-1, 0, new QStandardItem(subscribes[i].name));
        model->setItem(i-1, 1, new QStandardItem(subscribes[i].url));
        model->setItem(i-1, 2, new QStandardItem(subscribes[i].updateTime.toString("yyyy-MM-dd hh:mm:ss")));
    }
}

void SubscribeDialog::showSubNewDlg()
{
    if (!subscribeNewDialog) {
        subscribeNewDialog = new SubscribeNewDialog;

        connect(subscribeNewDialog.data(), &SubscribeNewDialog::newSubscribe, this, &SubscribeDialog::addSubscribe);
    }
}

void SubscribeDialog::addSubscribe(const Subscribe& newSubscribe)
{
    QList<Subscribe> subscribes = configurator.getSubscribes();
    int count = subscribes.count() - 1;

    QString subName = newSubscribe.name;
    QString subUrl = newSubscribe.url;
    // QMessageBox::information(this, "Add Subscribe", subName + subUrl);
    HttpUtil& http = HttpUtil::instance();
    QByteArray data = http.get(subUrl);
    if (!data.isEmpty()) {
        Configurator::saveClashConfig(subName, QString(data));
        subscribes.append(newSubscribe);
        configurator.setSubscribes(subscribes);
    } else {
        QMessageBox::warning(this, "Netork Error", "Remote config download failed, try again?");
        return;
    }
    auto* model = (QStandardItemModel*)this->tableView->model();
    QList<QStandardItem*> item;
    item << new QStandardItem(subName) << new QStandardItem(subUrl)
         << new QStandardItem(newSubscribe.updateTime.toString("yyyy-MM-dd hh:mm:ss"));
    model->appendRow(item);

    emit subscribeAdded(newSubscribe);
}

void SubscribeDialog::delSubscribe()
{
    QModelIndex index = this->tableView->currentIndex();
    if (!index.isValid())
        return;
    auto *model = (QStandardItemModel*)this->tableView->model();

    // QList<Subscribe> subscribes = configurator.getSubscribes();
    // subscribes.removeAt(index.row() + 1);
    // configurator.setSubscribes(subscribes);
    Subscribe subscribe = configurator.delSubscribeByIndex(index.row() + 1);

    model->removeRow(index.row());

    emit subscribeDeleted(index.row() + 1);
}

void SubscribeDialog::updateSubscribes()
{
    emit subscribesUpdate();
}

void SubscribeDialog::updateCell(const QModelIndex & indexA, const QModelIndex & indexB)
{
    int col = indexA.column();
    int row = indexB.row();
    QList<Subscribe> subscribes = configurator.getSubscribes();
    auto *model = (QStandardItemModel*)this->tableView->model();
    QString str = model->data(model->index(row, col)).toString();
    if (col > 1)
        return;
    if (col == 0)
        subscribes[row+1].name = str;
    if (col == 1)
        subscribes[row+1].url = str;
    configurator.setSubscribes(subscribes);
}

