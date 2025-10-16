#ifndef NES_H
#define NES_H

#include "shared/access_modifiers.h"
#include "cpu/cpu.h"
#include "ram.h"
#include "bus.h"
#include "cartridge/cartridge.h"

#include <cstdint>

class NES {
TEST_PUBLIC:
  CPU cpu;
  RAM ram;
  Cartridge cartridge;
  Bus bus;
public:
  NES();
  ~NES();

  void load_rom(const std::vector<uint8_t>& rom);
};

#endif  // NES_H
