#ifndef MACHINEDATA_H
#define MACHINEDATA_H

#include <QString>
#include "MachineState.h"

struct MachineData {
    int id = 0;
    QString name;
    MachineState state = MachineState::Open;
    QString claimer;
    int remaining = 0;
    int total = 0;
    QString cycle;
    QString start;
    QString end;
};

#endif // MACHINEDATA_H
