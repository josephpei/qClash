#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QVBoxLayout;
class QGroupBox;
class QFormLayout;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE


class SubscribeNewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SubscribeNewDialog(QWidget *parent = nullptr);

signals:
    void newSubscribe(const QStringList &sub);

private slots:
    void subscribeNewClicked();

private:
    QVBoxLayout *vLayout;
    QLineEdit *subName;
    QLineEdit *subUrl;
    QGroupBox *formGroupBox;
    QFormLayout *formLayout;
    QDialogButtonBox *buttonBox;
};

