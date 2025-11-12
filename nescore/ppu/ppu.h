#ifndef PPU_H
#define PPU_H

#include "shared/access_modifiers.h"
#include "ppu/sprite_unit.h"
#include "bus.h"

#include <cstdint>

class PPU {
private:
TEST_PUBLIC:
  Bus* bus;

  // CPU accessible
  uint8_t PPUCTRL;    // $2000
  uint8_t PPUMASK;    // $2001
  uint8_t PPUSTATUS;  // $2002
  uint8_t OAMADDR;    // $2003
  uint8_t OAMDATA;    // $2004
  uint8_t PPUSCROLL;  // $2005
  uint8_t PPUADDR;    // $2006
  uint8_t PPUDATA;    // $2007

  // Internal registers
  uint16_t v;  // Used as 15 bits
  uint16_t t;  // used as 15 bits
  uint8_t x;   // used as 3 bits
  bool w;

  // Internal latches holding data for the next tile
  uint8_t upper_next;
  uint8_t lower_next;
  uint8_t attr_next;

  // Internal latches holding data for current tile
  uint16_t upper_shift;
  uint16_t lower_shift;
  bool attr_1;
  bool attr_2;
  uint8_t attr_shift_1;
  uint8_t attr_shift_2;

  uint8_t OAM[256];
  uint8_t secondary_OAM[64];

  SpriteUnit sprite_units[8];

  uint16_t scanline;
  uint16_t dot;

  uint8_t gen_pixel();
  void sprite_evaluation();

public:
  PPU(Bus* bus);

  void reset();

  uint8_t tick();
};

#endif  // PPU_H
