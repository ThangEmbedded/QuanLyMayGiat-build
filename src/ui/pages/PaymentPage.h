#ifndef PAYMENTPAGE_H
#define PAYMENTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "widgets/HeaderWidget.h"
#include "widgets/QRWidget.h"

class PaymentPage : public QWidget {
    Q_OBJECT
public:
    explicit PaymentPage(QWidget *parent = nullptr);

    void setMachine(int machineId, const QString &cycle, int price);
    int getMachineId() const { return m_machineId; }

signals:
    void backRequested();
    void paymentConfirmed(int machineId, const QString &roomNumber);

private:
    HeaderWidget *m_header;
    int m_machineId;
    QString m_cycle;
    int m_price;

    QLineEdit *m_roomInput;
    QPushButton *m_confirmButton;
    QRWidget *m_qrWidget;

    void handleKeyPress(const QString &key);
};

#endif // PAYMENTPAGE_H
