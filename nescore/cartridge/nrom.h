#ifndef NROM_H
#define NROM_H

#include "cartridge/mapper.h"

#include <cstdint>
#include <vector>

class NROM : public Mapper {
private:
  uint8_t* prg_rom_bank1;
  uint8_t* prg_rom_bank2;

  uint8_t* chr_rom_bank1;

public:
  NROM(const std::vector<uint8_t>& rom);
  ~NROM();

  std::optional<uint8_t> read(uint16_t address, uint8_t source) override;
  void write(uint16_t address, uint8_t data, uint8_t source) override;
};

#endif  // NROM_H
