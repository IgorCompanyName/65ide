#include "mos6502.h"
#include <iostream>

uint8_t* ram;

uint8_t readFunc(uint16_t addr) {
    return ram[addr];
}
void writeFunc(uint16_t addr, uint8_t data) {
    ram[addr] = addr;
}

int main() {
    ram = (uint8_t*)malloc(65536);

    return 0;
}