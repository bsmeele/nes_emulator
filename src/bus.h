#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "ram.h"

class Bus {
private:
// TODO: add devices
  RAM* ram;
  uint8_t open_bus;
public:
  Bus();
  ~Bus();
  void mount_ram(RAM* ram);

  uint8_t read(uint16_t address);
  void write(uint16_t address, uint8_t data);
};

#endif  // BUS_H
