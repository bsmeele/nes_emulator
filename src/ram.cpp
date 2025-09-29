#include "ram.h"

#include <iostream>
#include <cassert>

RAM::RAM() {}
RAM::~RAM() {}

uint8_t RAM::read(uint16_t address) {
  if (address >= 0x2000) {
    // Outside of ram memory map. TODO: handling
    std::cout << "Outside of RAM range" << std::endl;
    assert(false);
    return 0x00;
  }

  return this->ram[address & 0xE7FF];  // $0800-$1FFF are mirrors of $0000-$07FF
}

void RAM::write(uint16_t address, uint8_t data) {
  // Outside of ram memory map. TODO: handling
  if (address >= 0x2000) {
    std::cout << "Outside of RAM range" << std::endl;
    assert(false);
    return;
  }

  this->ram[address & 0xE7FF] = data;  // $0800-$1FFF are mirrors of $0000-$07FF
}
