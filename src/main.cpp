#include "nes.h"

#include <iostream>

int main() {
  NES nes = NES();
  nes.bus.write(0x0001, 1);
  nes.bus.write(0x0802, 2);
  nes.bus.write(0x1003, 3);
  nes.bus.write(0x1804, 5);

  std::cout << "Reading from $0001 $0002 $0003 $0004" << std::endl;
  std::cout << +nes.bus.read(0x0001)
    << " " << +nes.bus.read(0x0002)
    << " " << +nes.bus.read(0x0003)
    << " " << +nes.bus.read(0x0004) << std::endl << std::endl;

  std::cout << "Reading from $0801 $0802 $0803 $0804" << std::endl;
  std::cout << +nes.bus.read(0x0801)
    << " " << +nes.bus.read(0x0802)
    << " " << +nes.bus.read(0x0803)
    << " " << +nes.bus.read(0x0804) << std::endl << std::endl;

  std::cout << "Reading from $1001 $1002 $1003 $1005" << std::endl;
  std::cout << +nes.bus.read(0x1001)
    << " " << +nes.bus.read(0x1002)
    << " " << +nes.bus.read(0x1003)
    << " " << +nes.bus.read(0x1004) << std::endl << std::endl;

  std::cout << "Reading from $1801 $1802 $1803 $1805" << std::endl;
  std::cout << +nes.bus.read(0x1801)
    << " " << +nes.bus.read(0x1802)
    << " " << +nes.bus.read(0x1803)
    << " " << +nes.bus.read(0x1804) << std::endl << std::endl;

  return 0;
}
