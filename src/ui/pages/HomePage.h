#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include "widgets/HeaderWidget.h"
#include "widgets/MachineCardWidget.h"
#include "models/MachineData.h"

class HomePage : public QWidget {
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

    void updateMachines(const QList<MachineData> &machines);

signals:
    void machineSelected(int machineId);
    void adminRequested();

private:
    HeaderWidget *m_header = nullptr;
    QPushButton *m_adminButton = nullptr;
    QList<MachineCardWidget*> m_cards;
};

#endif // HOMEPAGE_H
