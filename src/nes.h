#ifndef NES_H
#define NES_H

#include <cstdint>
#include "cpu.h"
#include "ram.h"
#include "bus.h"
#include "cartridge.h"

class NES {
private:
public:
  CPU cpu;
  RAM ram;
  Cartridge cartridge;
  Bus bus;
public:
  NES();
  ~NES();
};

#endif  // NES_H
