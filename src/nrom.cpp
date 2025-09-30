#include "nrom.h"

NROM::NROM(const std::vector<uint8_t>& rom) {  // TODO
  bank1 = new uint8_t[0x4000];
  bank2 = nullptr;
}

NROM::~NROM() {
  delete[] bank1;
  delete[] bank2;
}

std::optional<uint8_t> NROM::read(uint16_t address) {
  return {};  // TODO
}

void NROM::write(uint16_t address, uint8_t data) {
  // TODO
}
