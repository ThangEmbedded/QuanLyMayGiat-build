#pragma once

namespace relay
{

enum class RelayId
{
    MG1,
    MG2,
    MG3,
    MG4
};

class RelayDriver
{
public:
    bool init();

    bool on(RelayId id);

    bool off(RelayId id);

    bool toggle(RelayId id);
};

}