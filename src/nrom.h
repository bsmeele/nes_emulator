#ifndef NROM_H
#define NROM_H

#include "mapper.h"

#include <cstdint>
#include <vector>

class NROM : public Mapper {
private:
  uint8_t* bank1;
  uint8_t* bank2;
  
public:
  NROM(const std::vector<uint8_t>& rom);
  ~NROM();

  std::optional<uint8_t> read(uint16_t address) override;
  void write(uint16_t address, uint8_t data) override;
};

#endif  // NROM_H
