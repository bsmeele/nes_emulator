#include "nes.h"

NES::NES() : bus(), cpu(&bus), ram(), cartridge() {
  this->bus.mount(&ram);
  this->bus.mount(&cartridge);
}

NES::~NES() {}
