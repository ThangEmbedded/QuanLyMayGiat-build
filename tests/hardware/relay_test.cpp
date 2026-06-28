#include "../../src/drivers/relay/relay.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace relay;

int main()
{
    RelayDriver relay;

    relay.init();

    std::cout << "Relay Test Start\n";

    RelayId list[] =
    {
        RelayId::MG1,
        RelayId::MG2,
        RelayId::MG3,
        RelayId::MG4
    };

    for(auto id : list)
    {
        std::cout << "ON\n";

        relay.on(id);

        std::this_thread::sleep_for(
            std::chrono::seconds(2));

        std::cout << "OFF\n";

        relay.off(id);

        std::this_thread::sleep_for(
            std::chrono::seconds(1));
    }

    std::cout << "Finish\n";
}