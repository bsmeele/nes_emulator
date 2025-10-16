#ifndef RAM_H
#define RAM_H

#include "shared/access_modifiers.h"
#include "memory_device.h"

#include <cstdint>

class RAM : public MemoryDevice {
private:
TEST_PUBLIC:
  uint8_t ram[2048];  // Unreliable power-up state on actual hardware
  
public:
  RAM();
  ~RAM();

  std::optional<uint8_t> read(uint16_t address) override;
  void write(uint16_t address, uint8_t data) override;
};

#endif  // RAM_H
