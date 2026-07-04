#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <QString>

enum class MachineState {
    Open,
    Running,
    Offline,
    Error
};

inline QString machineStateToString(MachineState state) {
    switch (state) {
    case MachineState::Open:    return "open";
    case MachineState::Running: return "running";
    case MachineState::Offline: return "offline";
    case MachineState::Error:   return "error";
    }
    return "error";
}

inline QString machineStateDisplayText(MachineState state) {
    switch (state) {
    case MachineState::Open:    return "Còn trống";
    case MachineState::Running: return "Đang chạy";
    case MachineState::Offline: return "Ngoại tuyến";
    case MachineState::Error:   return "Lỗi";
    }
    return "Lỗi";
}

#endif // MACHINESTATE_H
