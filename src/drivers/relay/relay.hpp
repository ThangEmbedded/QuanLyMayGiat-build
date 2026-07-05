#pragma once

namespace relay
{

enum class RelayId
{
    MG1 = 0,
    MG2,
    MG3,
    MG4,
    Count
};

class RelayDriver
{
public:
    bool init();
    void deinit();

    bool on(RelayId id);
    bool off(RelayId id);
    bool toggle(RelayId id);
    bool isOn(RelayId id) const;

private:
    bool initialized_{false};
    bool relay_state_[static_cast<int>(RelayId::Count)]{false, false, false, false};

    static bool isValid(RelayId id);
    static unsigned int getPin(RelayId id);
    bool set(RelayId id, bool enabled);
};

} // namespace relay
