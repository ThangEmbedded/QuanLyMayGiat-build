#ifndef MACHINEDETAILPAGE_H
#define MACHINEDETAILPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include "widgets/HeaderWidget.h"
#include "models/MachineData.h"

class MachineDetailPage : public QWidget {
    Q_OBJECT
public:
    explicit MachineDetailPage(QWidget *parent = nullptr);

    void setMachine(const MachineData &machine);
    QString getSelectedCycle() const { return m_selectedCycle; }
    int getSelectedPrice() const { return m_selectedPrice; }
    int getSelectedDuration() const { return m_selectedDuration; }
    int getMachineId() const { return m_machineId; }

signals:
    void backRequested();
    void paymentRequested(int machineId);

private:
    HeaderWidget *m_header;
    int m_machineId;
    QString m_selectedCycle;
    int m_selectedPrice;
    int m_selectedDuration;

    QLabel *m_machineTitleLabel;
    QLabel *m_cycleLabel;
    QLabel *m_priceLabel;
    QLabel *m_durationLabel;
    QPushButton *m_payButton;

    void setupCycleButton(QPushButton *btn, const QString &title, int price, int duration);
};

#endif // MACHINEDETAILPAGE_H
