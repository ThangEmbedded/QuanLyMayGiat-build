#ifndef TRANSITIONSTACKEDWIDGET_H
#define TRANSITIONSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class TransitionStackedWidget : public QStackedWidget {
    Q_OBJECT
public:
    explicit TransitionStackedWidget(QWidget *parent = nullptr);
    void setCurrentIndexWithFade(int index, int durationMs = 250);

private:
    QLabel *overlayLabel;
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *fadeAnimation;
};

#endif // TRANSITIONSTACKEDWIDGET_H
