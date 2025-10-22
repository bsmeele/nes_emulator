#include "ram.h"

#include <cassert>

RAM::RAM() {}
RAM::~RAM() {}

std::optional<uint8_t> RAM::read(uint16_t address) {
  if (address >= 0x2000) {
    // Outside of ram memory map
    return {};
  }

  return this->ram[address & 0xE7FF];  // $0800-$1FFF are mirrors of $0000-$07FF
}

void RAM::write(uint16_t address, uint8_t data) {
  if (address >= 0x2000) {
    // Outside of ram memory map
    return;
  }

  this->ram[address & 0xE7FF] = data;  // $0800-$1FFF are mirrors of $0000-$07FF
}
