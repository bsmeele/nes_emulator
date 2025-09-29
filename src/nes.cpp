#include "nes.h"

NES::NES() : bus(), cpu(&bus), ram() {
  this->bus.mount_ram(&ram);
}
NES::~NES() {}
