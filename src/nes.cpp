#include "nes.h"

NES::NES() : bus(), cpu(&bus), ram(), cartridge() {
  this->bus.mount(&ram);
  this->bus.mount(&cartridge);
}

NES::~NES() {}

void NES::load_rom(const std::vector<uint8_t>& rom) {
  this->cartridge.load_rom(rom);
}
