#ifndef BUS_H
#define BUS_H

#include "memory_device.h"

#include <cstdint>
#include <vector>

class Bus {
private:
  std::vector<MemoryDevice*> devices;
  uint8_t open_bus;

public:
  Bus();
  ~Bus();

  void mount(MemoryDevice* device);

  uint8_t read(uint16_t address);
  void write(uint16_t address, uint8_t data);
};

#endif  // BUS_H
