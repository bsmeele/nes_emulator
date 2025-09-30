#ifndef MAPPER_H
#define MAPPER_H

#include "memory_device.h"

#include <cstdint>

class Mapper : public MemoryDevice {
private:
public:
  virtual std::optional<uint8_t> read(uint16_t address) = 0;
  virtual void write(uint16_t address, uint8_t data) = 0;
};

#endif  // MAPPER_H
