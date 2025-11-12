#include "nes.h"

NES::NES() : cpu_bus(0), cpu(&cpu_bus), ppu_bus(1), ppu(&ppu_bus), ram(), cartridge() {
  this->cpu_bus.mount(&ram);
  this->cpu_bus.mount(&cartridge);

  this->ppu_bus.mount(&cartridge);
}

NES::~NES() {}

void NES::load_rom(const std::vector<uint8_t>& rom) {
  this->cartridge.load_rom(rom);
}
