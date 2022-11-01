#include "mos6502.h"
#include <cstdint>
#include <iostream>
#include <thread>

int stopped = 0;

uint8_t dullRead(uint16_t addr) {
    return 0;
}
void dullWrite(uint16_t addr, uint8_t data) {
    int a = addr;
}
void run_cpu(mos6502 cpu, uint64_t &cycles) {
    while(!stopped) {
        cpu.Run(1, cycles);
        for(int i = 0; i < 3000; i++) {} // seems like something more precise is impossible
    } // TODO: find better way to delay loop, maybe there is...
}
int test_speed() {
    uint64_t cycles = 0;
    mos6502 cpu(dullRead, dullWrite);
    cpu.Reset();

    std::cout << "Running test of cpu speed..." << "\n";

    std::thread cpu_thread(run_cpu, cpu, std::ref(cycles));

    std::this_thread::sleep_for(std::chrono::seconds(1));
    stopped = 1;
    cpu_thread.join();
    if(cycles >= 1e6 && cycles <= 1e7) { // in the middle of 1M to 10M
        return 0;
    }
    std::cerr << "Cycles elapsed: " << cycles << '\n' << "Test failed!" << std::endl;
    return -1;
}
int main() {
    return test_speed();
}