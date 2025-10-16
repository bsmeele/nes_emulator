#include "bus.h"

Bus::Bus() {
  this->open_bus = 0x00;  // Not sure what the power-on behaviour of the open bus would be. Possibly it's random, similar to RAM
}

Bus::~Bus() {}

void Bus::mount(MemoryDevice* device) {
  this->devices.push_back(device);
}

uint8_t Bus::read(uint16_t address) {
  for (auto device : devices) {
    // TODO: Bus conflict
    std::optional<uint8_t> data = device->read(address);
    if (data) { return *data; }
  }
  
  return this->open_bus;
}

void Bus::write(uint16_t address, uint8_t data) {

  for (auto device : devices) {
    device->write(address, data);
  }
}
