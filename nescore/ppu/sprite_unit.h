#ifndef SPRITE_UNIT_H
#define SPRITE_UNIT_H

#include <cstdint>

struct SpriteUnit {
  uint8_t upper;
  uint8_t lower;

  uint8_t palette;  // Used as 2 bits;
  bool priority;  // false = foreground, true = behind background

  uint8_t counter;

  SpriteUnit();
  SpriteUnit(uint8_t upper, uint8_t lower, uint8_t attribute, uint8_t x_position);

  uint8_t tick();
};

#endif  // SPRITE_UNIT_H
