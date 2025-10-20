#include "cartridge/nrom.h"

#include <iostream>
#include <sstream>
#include <cstring>

NROM::NROM(const std::vector<uint8_t>& rom) {  // TODO
  if (rom[0] != 0x4E || rom[1] != 0x45 || rom[2] != 0x53 || rom[3] != 0x1A) {
    
    throw std::runtime_error("Invalid .nes format");
    return;
  }

  uint8_t mapper_num = (rom[7] & 0xF0) | ((rom[6] & 0xF0) >> 4);
  if (mapper_num != 0) {
      std::ostringstream oss;
      oss << "Invalid mapper number " << mapper_num << " for NROM";
      throw std::invalid_argument(oss.str());
  }

  uint8_t prg_rom_size = rom[4];
  if (prg_rom_size > 2) {
    std::ostringstream oss;
    oss << "Invalid PRG ROM size " << prg_rom_size << " for NROM";
    throw std::invalid_argument(oss.str());
  }

  uint8_t chr_rom_size = rom[5];
  if (chr_rom_size > 1) {
    std::ostringstream oss;
    oss << "Invalid CHR ROM size " << chr_rom_size << " for NROM";
    throw std::invalid_argument(oss.str());
  }

  bool trainer_present = (rom[6] & 0x04) > 0;

  this->prg_rom_bank1 = new uint8_t[0x4000];
  if (prg_rom_size > 1) {
    this->prg_rom_bank2 = new uint8_t[0x4000];
  } else {
    this->prg_rom_bank2 = nullptr;
  }

  if (prg_rom_size > 0) {
    size_t  offset = 16 + 512 * trainer_present;
    if (offset + 0x4000 > rom.size()) {
      throw std::out_of_range("Not enough PRG ROM data in file");
    }
    std::memcpy(this->prg_rom_bank1, rom.data() + offset, 0x4000);
  }

  if (prg_rom_size > 1) {
    size_t  offset = 16 + 512 * trainer_present + 0x4000;
    if (offset + 0x4000 > rom.size()) {
      throw std::out_of_range("Not enough PRG ROM data in file");
    }
    std::memcpy(this->prg_rom_bank2, rom.data() + offset, 0x4000);
  }

  this->chr_rom_bank1 = new uint8_t[0x2000];
  if (chr_rom_size > 0) {
    size_t offset = 16 + 512 * trainer_present + 0x4000 * prg_rom_size;
    if (offset + 0x2000 > rom.size()) {
      throw std::out_of_range("Not enough CHR ROM data in file");
    std::memcpy(this->chr_rom_bank1, rom.data() + offset, 0x2000);
    }
  }
}

NROM::~NROM() {
  delete[] this->prg_rom_bank1;
  delete[] this->prg_rom_bank2;

  delete[] this->chr_rom_bank1;
}

std::optional<uint8_t> NROM::read(uint16_t address) {
  if (address < 0x8000) {
    // Outside PRG ROM range
    return {};
  }

  if (address <= 0xBFFF) {
    return this->prg_rom_bank1[address & 0x3FFF];
  }

  if (this->prg_rom_bank2) {
    return this->prg_rom_bank2[address & 0x3FFF];
  } else {
    return this->prg_rom_bank1[address & 0x3FFF];
  }

  // Unreachable
  return {};
}

void NROM::write(uint16_t address, uint8_t data) {
  // contains no writable memory
}
