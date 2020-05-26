#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QLabel;
QT_END_NAMESPACE

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

private:
    QVBoxLayout *vLayout;
    QLabel *aboutLabel;
};