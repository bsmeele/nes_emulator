#include "nes.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

int main() {
  NES nes = NES();

  std::string filename = "../roms/Mario Bros. (World).nes";
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (!file) {
    throw std::runtime_error("Could not open file: " + filename);
  }
  
  std::streamsize filesize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> rom(filesize);

  if (!file.read(reinterpret_cast<char*>(rom.data()), filesize)) {
    throw std::runtime_error("Could not read file: " + filename);
  }

  nes.load_rom(rom);

#ifdef TESTING
  std::cout << "Reading the first 4 bytes from bank1 of the cartridge" << std::endl;
  std::cout << std::hex << std::uppercase
    << static_cast<int>(nes.cpu_bus.read(0x8000))
    << " " << static_cast<int>(nes.cpu_bus.read(0x8001))
    << " " << static_cast<int>(nes.cpu_bus.read(0x8002))
    << " " << static_cast<int>(nes.cpu_bus.read(0x8003))
    << " " << static_cast<int>(nes.cpu_bus.read(0x8004))
    << std::dec << std::endl;

  std::cout << "Reading the first 4 bytes from bank2 of the cartridge" << std::endl;
  std::cout << std::hex << std::uppercase
    << static_cast<int>(nes.cpu_bus.read(0xC000))
    << " " << static_cast<int>(nes.cpu_bus.read(0xC001))
    << " " << static_cast<int>(nes.cpu_bus.read(0xC002))
    << " " << static_cast<int>(nes.cpu_bus.read(0xC003))
    << " " << static_cast<int>(nes.cpu_bus.read(0xC004))
    << std::dec << std::endl;
#endif

  return 0;
}
