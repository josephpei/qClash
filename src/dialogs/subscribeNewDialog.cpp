#include <QtWidgets>

#include "subscribeNewDialog.h"

SubscribeNewDialog::SubscribeNewDialog(QWidget *parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);

    //ui
    vLayout = new QVBoxLayout;
    subName = new QLineEdit;
    subUrl = new QLineEdit;
    formLayout = new QFormLayout;
    formGroupBox = new QGroupBox;
    formLayout->addRow(tr("Subscribe Name"), subName);
    formLayout->addRow(tr("Subscribe Url"), subUrl);
    formGroupBox->setLayout(formLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SubscribeNewDialog::subscribeNewClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    vLayout->addWidget(formGroupBox);
    vLayout->addWidget(buttonBox);
    setLayout(vLayout);
    setMinimumWidth(450);
    show();
}

void SubscribeNewDialog::subscribeNewClicked()
{
    // QStringList subscribe;
    // subscribe << this->subName->text() << this->subUrl->text();
    // emit newSubscribe(subscribe);
    QString name = subName->text();
    QString url = subUrl->text();
    if (!name.isEmpty() && name != "config" && !url.isEmpty()) {
        Subscribe subscribe(name, url);
        emit newSubscribe(subscribe);
        accept();
    }
}
