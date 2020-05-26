#include <QVBoxLayout>
#include <QLabel>

#include "aboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
    vLayout(new QVBoxLayout),
    aboutLabel(new QLabel)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("About qClash");

    const QString aboutText = QString(
        "<p style=\"white-space: pre-wrap;\">"
        "An gui client for clash.\n\n"
        "</p>");

    aboutLabel->setTextFormat(Qt::RichText);
    aboutLabel->setText(aboutText);
    vLayout->addWidget(aboutLabel);
    setLayout(vLayout);
}

AboutDialog::~AboutDialog()
{
    delete vLayout;
    delete aboutLabel;
}