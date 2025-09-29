#ifndef NES_H
#define NES_H

#include <cstdint>
#include "cpu.h"
#include "ram.h"
#include "bus.h"

class NES {
private:
public:
  CPU cpu;
  RAM ram;
  Bus bus;
public:
  NES();
  ~NES();
};

#endif  // NES_H
