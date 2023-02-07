#ifndef QCLASH_COLLAPSEWIDGET_H
#define QCLASH_COLLAPSEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QFrame>


class CollapseWidget : public QWidget {
    Q_OBJECT

protected:
    QGridLayout mainLayout{};
    QHBoxLayout headerLayout{};
    QToolButton toggleButton{};
    QFrame headerLine{};
    QParallelAnimationGroup toggleAnimation{};
    QScrollArea contentArea{};
    int duration;
    QString title;
    
    void updateHeights();

public:
    explicit CollapseWidget(const QString &title = "", int duration = 100, QWidget *parent = nullptr);
    void setHeaderLayout();
    void setContentLayout(QLayout &contentLayout);

    void toggleContentShown(bool visible);

};

#endif // QCLASH_COLLAPSEWIDGET_H
