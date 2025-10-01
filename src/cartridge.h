#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "memory_device.h"
#include "mapper.h"

#include <cstdint>
#include <vector>
#include <memory>

class Cartridge : public MemoryDevice {
private:
  std::unique_ptr<Mapper> mapper;

public:
  Cartridge();
  ~Cartridge();

  void load_rom(const std::vector<uint8_t>& rom);

  std::optional<uint8_t> read(uint16_t address) override;
  void write(uint16_t address, uint8_t data) override;
};

#endif  // CARTRIDGE_H
