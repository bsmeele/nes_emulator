#include "ppu/sprite_unit.h"

SpriteUnit::SpriteUnit() : upper(0x00), lower(0x00), palette(0x00), priority(true), counter(0xFF) {};

SpriteUnit::SpriteUnit(uint8_t upper, uint8_t lower, uint8_t attribute, uint8_t x_position) {
  this->upper = upper;
  this->lower = lower;
  this->palette = attribute & 0x03;
  this->priority = ((attribute & 0x20) != 0);
  this->counter = x_position;
}

// Output is the lower byte of the address of the pixel colour
//   000SAAPP
//   ||||||||
//   ||||||++- Colour within palette. Obtained from pattern data
//   ||||++--- Palette number. Obtained from attribute data
//   |||+----- Priority. 0 for foreground, 1 for background
//   +++------ set to 0
uint8_t SpriteUnit::tick() {
  uint8_t out = 0x00;
  
  if (this->counter == 0) {
    out |= (this->upper & 0x80) >> 6;
    out |= (this->lower & 0x80) >> 7;
    out |= palette << 2;
    out |= 0x10 * priority;

    this->upper = this->upper << 1;
    this->lower = this->lower << 1;

  } else {
    this->counter -= 1;
  }

  return out;
}
