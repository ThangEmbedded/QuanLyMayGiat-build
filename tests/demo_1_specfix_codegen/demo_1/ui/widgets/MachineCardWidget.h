#ifndef MACHINECARDWIDGET_H
#define MACHINECARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "CircularProgressRing.h"

class MachineCardWidget : public QWidget {
    Q_OBJECT
public:
    explicit MachineCardWidget(int id, QWidget *parent = nullptr);

    int getMachineId() const { return m_id; }
    QString getState() const { return m_state; }

    void setMachineData(const QString &name, const QString &state, const QString &claimer = "", int remaining = 0, int total = 45);

signals:
    void clicked(int machineId);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void runningPulseOff();
    void runningPulseOn();
    void finishedBlinkStep();
    void finishedHoldDone();

private:
    void applyStatusAnimation(const QString &state);
    void stopStatusAnimation();
    void startRunningAnimation();
    void startFinishedBlinkCycle();

    int m_id;
    QString m_state;
    QString m_visualState;
    QLabel *m_nameLabel;
    CircularProgressRing *m_progressRing;
    QLabel *m_statusLabel;
    QLabel *m_claimerLabel;
    QPushButton *m_claimButton;

    QTimer m_runningPulseTimer;
    QTimer m_runningRestoreTimer;
    QTimer m_finishedBlinkTimer;
    QTimer m_finishedHoldTimer;
    int m_finishedBlinkStep = 0;
};

#endif // MACHINECARDWIDGET_H
