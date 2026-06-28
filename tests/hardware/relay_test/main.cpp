#include "relay.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace relay; //

int main() {
    std::cout << "=== HE THONG KIEM THU PHAN CUNG: RELAY MODULE ===" << std::endl;

    RelayDriver machineRelay;

    // Khởi tạo relay (Hàm init của Relay sẽ tự gọi init của Gpio bên dưới)
    if (!machineRelay.init()) {
        std::cerr << "Loi: Khong the khoi tao module Relay!" << std::endl;
        return 1;
    }
    std::cout << "-> Khoi tao module Relay thanh cong." << std::endl;

    std::cout << "\n--- BAT DAU CHU TRINH TOGGLE TEST ---" << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << "[Chu ky " << i + 1 << "/5] -> ON tat ca may giat" << std::endl;
        
        // Gọi các hàm logic của tầng Relay (ví dụ: turnOn, turnOff hoặc write)
        machineRelay.on(RelayId::MG1);
    machineRelay.on(RelayId::MG2);
    machineRelay.on(RelayId::MG3);
    machineRelay.on(RelayId::MG4); 
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "[Chu ky " << i + 1 << "/5] -> OFF tat ca may giat" << std::endl;
        machineRelay.off(RelayId::MG1);
    machineRelay.off(RelayId::MG2);
    machineRelay.off(RelayId::MG3);
    machineRelay.off(RelayId::MG4);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "=== KET THUC QUY TRINH TEST RELAY ===" << std::endl;
    return 0;
}