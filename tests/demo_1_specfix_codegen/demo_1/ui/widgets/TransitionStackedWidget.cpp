#include "TransitionStackedWidget.h"
#include <QPixmap>

TransitionStackedWidget::TransitionStackedWidget(QWidget *parent)
    : QStackedWidget(parent), overlayLabel(nullptr), opacityEffect(nullptr), fadeAnimation(nullptr) {
}

void TransitionStackedWidget::setCurrentIndexWithFade(int index, int durationMs) {
    QWidget *oldWidget = currentWidget();
    QWidget *newWidget = widget(index);

    if (!oldWidget || !newWidget || oldWidget == newWidget) {
        setCurrentIndex(index);
        return;
    }

    // Capture the old widget's appearance
    QPixmap pixmap = oldWidget->grab();

    // Create or reuse overlay label
    if (overlayLabel) {
        overlayLabel->deleteLater();
    }
    overlayLabel = new QLabel(this);
    overlayLabel->setPixmap(pixmap);
    overlayLabel->setGeometry(0, 0, width(), height());
    overlayLabel->show();

    // Setup opacity effect
    opacityEffect = new QGraphicsOpacityEffect(overlayLabel);
    overlayLabel->setGraphicsEffect(opacityEffect);

    // Switch index underneath
    setCurrentIndex(index);

    // Animate opacity of overlay from 1.0 (visible) to 0.0 (transparent)
    fadeAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    fadeAnimation->setDuration(durationMs);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    fadeAnimation->setEasingCurve(QEasingCurve::OutQuad);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        if (overlayLabel) {
            overlayLabel->hide();
            overlayLabel->deleteLater();
            overlayLabel = nullptr;
        }
    });

    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
