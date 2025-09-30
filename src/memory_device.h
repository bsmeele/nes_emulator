#ifndef MEMORY_DEVICE_H
#define MEMORY_DEVICE_H

#include <cstdint>
#include <optional>

class MemoryDevice {
private:
public:
  virtual std::optional<uint8_t> read(uint16_t address) = 0;
  virtual void write(uint16_t address, uint8_t data) = 0;
};

#endif  // MEMORY_DEVICE_H
