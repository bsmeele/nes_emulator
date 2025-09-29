#include "bus.h"

Bus::Bus() {
  this->ram = nullptr;

  this->open_bus = 0x00;  // Not sure what the power-on behaviour of the open bus would be. Possibly it's random, similar to RAM
}
Bus::~Bus() {}

void Bus::mount_ram(RAM* ram) {
  this->ram = ram;
}

uint8_t Bus::read(uint16_t address) {
  if (address <= 0x1FFF) {
    uint8_t data = this->ram->read(address);
    this->open_bus = data;
    return data;
  }
  
  return this->open_bus;
}

void Bus::write(uint16_t address, uint8_t data) {
  if (address <= 0x1FFF) {
    this->ram->write(address, data);
    return;
  }

  return;
}
