#ifndef RUNNINGSTATUSPAGE_H
#define RUNNINGSTATUSPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "widgets/HeaderWidget.h"
#include "widgets/CircularProgressRing.h"
#include "models/MachineData.h"

class RunningStatusPage : public QWidget {
    Q_OBJECT
public:
    explicit RunningStatusPage(QWidget *parent = nullptr);

    void setMachine(const MachineData &machine);
    int getMachineId() const { return m_machineId; }

signals:
    void backRequested();

private:
    HeaderWidget *m_header;
    int m_machineId;

    CircularProgressRing *m_progressRing;
    QLabel *m_machineNameLabel;
    QLabel *m_stateBadgeLabel;

    QLabel *m_userVal;
    QLabel *m_cycleVal;
    QLabel *m_startVal;
    QLabel *m_endVal;

    QPushButton *m_notifyBtn;
    QPushButton *m_homeBtn;
};

#endif // RUNNINGSTATUSPAGE_H
