#ifndef CONFIRMSTARTPAGE_H
#define CONFIRMSTARTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "widgets/HeaderWidget.h"
#include "models/MachineData.h"

class ConfirmStartPage : public QWidget {
    Q_OBJECT
public:
    explicit ConfirmStartPage(QWidget *parent = nullptr);

    void setMachineAndCycle(const MachineData &machine,
                            const QString &cycleName,
                            int durationMinutes,
                            int priceVnd);

signals:
    void backRequested();
    void startConfirmed(int machineId, const QString &roomNumber);

private:
    HeaderWidget *m_header = nullptr;
    QLabel *m_machineLabel = nullptr;
    QLabel *m_cycleLabel = nullptr;
    QLabel *m_durationLabel = nullptr;
    QLabel *m_priceLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLineEdit *m_roomInput = nullptr;
    QPushButton *m_confirmButton = nullptr;
    QPushButton *m_cancelButton = nullptr;

    int m_machineId = 0;

    void updateConfirmState();
};

#endif // CONFIRMSTARTPAGE_H
