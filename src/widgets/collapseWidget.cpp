#include "collapseWidget.h"
#include <QDebug>
#include <QPropertyAnimation>

CollapseWidget::CollapseWidget(const QString& title, int duration, QWidget* parent)
: QWidget(parent), duration(duration)
{
    toggleButton.setStyleSheet("QToolButton {border: none;}");
    //toggleButton.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton.setArrowType(Qt::ArrowType::RightArrow);
    //toggleButton.setText(title);
    toggleButton.setCheckable(true);
    toggleButton.setChecked(false);
    toggleButton.setObjectName("collapsableWidgetToggleButton");
    toggleButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    headerLine.setFrameShape(QFrame::HLine);
    headerLine.setFrameShadow(QFrame::Sunken);
    headerLine.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    contentArea.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //Start out collapsed
    contentArea.setMaximumHeight(0);
    contentArea.setMinimumHeight(0);
    //contentArea.setObjectName("collapsableWidgetContent");

    //Let the entire widget grow and shrink with its content
    toggleAnimation.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggleAnimation.addAnimation(new QPropertyAnimation(&contentArea, "minimumHeight"));
    toggleAnimation.addAnimation(new QPropertyAnimation(&contentArea, "maximumHeight"));
    connect(&toggleAnimation, &QParallelAnimationGroup::finished, this, &CollapseWidget::animationFinished);

    //Don't waste space
    //mainLayout.setSpacing(0);
    //mainLayout.setMargin(0);
    mainLayout.setVerticalSpacing(0);
    mainLayout.setContentsMargins(0, 0, 0, 0);

    //mainLayout.addWidget(&toggleButton);
    //mainLayout.addWidget(&contentArea);
    int row = 0;
    mainLayout.addWidget(&toggleButton, row, 0, 1, 1, Qt::AlignLeft);
    mainLayout.addWidget(&headerLine, row, 2, 1, 1);
    mainLayout.addWidget(&contentArea, row+1, 0, 1, 3);

    setLayout(&mainLayout);

    QObject::connect(&toggleButton, &QToolButton::clicked, this, &CollapseWidget::toggleContentShown);
}

void CollapseWidget::setHeaderLayout(QHBoxLayout& headerLayout)
{
    headerLayout.addWidget(&toggleButton);
    headerLayout.addWidget(&headerLine);
    mainLayout.addLayout(&headerLayout, 0, 0, 1, 3);
}

void CollapseWidget::setContentLayout(QLayout& contentLayout)
{
    delete contentArea.layout();
    //contentLayout.setContentsMargins(8, 4, 4, 4);
    contentArea.setLayout(&contentLayout);
    
    //Fetch the current and target heights
    const int collapsedHeight = sizeHint().height() - contentArea.maximumHeight();
    const int contentHeight = contentLayout.sizeHint().height();
    //const int contentHeight = contentArea.layout()->sizeHint().height() + contentArea.contentsMargins().top() + contentArea.contentsMargins().bottom();
    
    for (int i = 0; i < toggleAnimation.animationCount() - 1; i++) {
        QPropertyAnimation *spoilerAnimation = qobject_cast<QPropertyAnimation*>(toggleAnimation.animationAt(i));
        spoilerAnimation->setDuration(duration);
        spoilerAnimation->setStartValue(collapsedHeight);
        spoilerAnimation->setEndValue(collapsedHeight + contentHeight);
    }
    QPropertyAnimation* contentAnimation = qobject_cast<QPropertyAnimation*>(toggleAnimation.animationAt(2));
    contentAnimation->setDuration(duration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);
}

void CollapseWidget::toggleContentShown(bool visible)
{
    //The toggle button may not always be synced up with the visibility (eg: if this function was called in code)
    //So make sure it is
    //Also set the flippy arrow
    toggleButton.setChecked(visible);
    toggleButton.setArrowType(visible ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);

//    //Fetch the current and target heights
//    const int collapsedHeight = sizeHint().height() - contentArea.height();
//    const int contentHeight = contentArea.layout()->sizeHint().height() + contentArea.contentsMargins().top() + contentArea.contentsMargins().bottom();
//
//    //Setup all the animations
//    QPropertyAnimation *spoilerAnimation = static_cast<QPropertyAnimation*>(toggleAnimation.animationAt(0));
//    spoilerAnimation->setDuration(duration);
//    spoilerAnimation->setStartValue(collapsedHeight);
//    spoilerAnimation->setEndValue(collapsedHeight + contentHeight);
//
//    //Setup the contentArea minimumHeight animation
//    QPropertyAnimation *contentAnimation = static_cast<QPropertyAnimation*>(toggleAnimation.animationAt(1));
//    contentAnimation->setDuration(duration);
//    contentAnimation->setStartValue(0);
//    contentAnimation->setEndValue(contentHeight);
//    //Setup the contentArea maximumHeight animation
//    contentAnimation = static_cast<QPropertyAnimation*>(toggleAnimation.animationAt(2));
//    contentAnimation->setDuration(duration);
//    contentAnimation->setStartValue(0);
//    contentAnimation->setEndValue(contentHeight);
//
    //Kickstart the animation
    toggleAnimation.setDirection(visible ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggleAnimation.start();
}

void CollapseWidget::animationFinished() {
    if (toggleButton.isChecked()) {
        //Unset min/max heights when we're done
//        setMaximumHeight(QWIDGETSIZE_MAX);
//        contentArea.setMaximumHeight(QWIDGETSIZE_MAX);
//        contentArea.setMinimumHeight(0);
    }
}
