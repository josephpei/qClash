#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

#include "aboutDialog.h"
#include "BuildConfig.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
    layout(new QHBoxLayout),
    logoLabel(new QLabel),
    aboutLabel(new QLabel)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("About qClash");

    const QString aboutText = QString(
        "<p style=\"white-space: pre-wrap;\">"
        "An gui client for clash.\n"
        "Version %1\n"
        "</p>").arg(QCLASH_VERSION);

    QPixmap logo(":/assets/icons/qClash.png");
    logoLabel->setPixmap(logo);

    aboutLabel->setTextFormat(Qt::RichText);
    aboutLabel->setText(aboutText);

    layout->addWidget(logoLabel);
    layout->addWidget(aboutLabel);
    setLayout(layout);
}

AboutDialog::~AboutDialog()
{
    delete layout;
    delete aboutLabel;
}