#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QList>
#include "widgets/HeaderWidget.h"
#include "widgets/MachineCardWidget.h"

struct MachineData {
    int id;
    QString name;
    QString state; // "open", "running", "offline"
    QString claimer;
    int remaining;
    int total;
    QString cycle;
    QString start;
    QString end;
};

class HomePage : public QWidget {
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

    void updateMachines(const QList<MachineData> &machines);

signals:
    void machineSelected(int machineId);
    void adminRequested();

private:
    HeaderWidget *m_header;
    QList<MachineCardWidget*> m_cards;
};

#endif // HOMEPAGE_H
