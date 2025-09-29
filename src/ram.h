#ifndef RAM_H
#define RAM_H

#include <cstdint>

class RAM {
private:
  uint8_t ram[2048];  // Unreliable state on actual hardware
public:
  RAM();
  ~RAM();
  uint8_t read(uint16_t address);
  void write(uint16_t address, uint8_t data);
};

#endif  // RAM_H
