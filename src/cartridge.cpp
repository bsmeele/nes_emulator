#include "cartridge.h"

Cartridge::Cartridge() {
  this->mapper = nullptr;
}

Cartridge::~Cartridge() {}

void Cartridge::load_rom(const std::vector<uint8_t>& rom) {
  // this->mapper = std::make_unique<Mapper>(rom);  // TODO: replace with constructor of the actual mapper
}

std::optional<uint8_t> Cartridge::read(uint16_t address) {
  if (mapper) {
    return this->mapper->read(address);
  }
  return {};
}

void Cartridge::write(uint16_t address, uint8_t data) {
  if (mapper) {
    this->mapper->write(address, data);
  }
}
