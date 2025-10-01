#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "bus.h"

class CPU {
private:
  Bus* bus;

  uint8_t accumulator;
  uint8_t x_index;
  uint8_t y_index;
  uint8_t flag;             // NO1BDIZC
  uint8_t stack_pointer;
  uint16_t program_counter;
  uint8_t lockout_counter;

public:
  CPU(Bus* bus);
  ~CPU();

  void reset();
  void tick();
};

#endif  // CPU_H
