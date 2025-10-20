#include "cartridge/cartridge.h"
#include "cartridge/mapper_implementations.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

Cartridge::Cartridge() {
  this->mapper = nullptr;
}

Cartridge::~Cartridge() {}

void Cartridge::load_rom(const std::vector<uint8_t>& rom) {
  if (rom[0] != 0x4E || rom[1] != 0x45 || rom[2] != 0x53 || rom[3] != 0x1A) {
    throw std::invalid_argument("Invalid .nes format");
  }

  uint8_t mapper_num = (rom[7] & 0xF0) | ((rom[6] & 0xF0) >> 4);

  switch (mapper_num) {
    case 0:
      this->mapper = std::make_unique<NROM>(rom);
      break;
    default:
      std::ostringstream oss;
      oss << "Mapper " << mapper_num << " not supported";
      throw std::invalid_argument(oss.str());
      break;
  }
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
