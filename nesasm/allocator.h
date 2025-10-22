#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "shared/access_modifiers.h"

#include <cstdint>
#include <vector>
#include <utility>
#include <optional>

class Allocator {
private:
TEST_PUBLIC:
  std::vector<std::pair<uint16_t, uint16_t>> memory_blocks;
public:
  Allocator();

  std::optional<uint16_t> reserve_block(std::size_t size);
  std::optional<uint16_t> reserve_at(std::uint16_t location);
  std::optional<uint16_t> reserve_at(std::uint16_t location, std::size_t size);

  // std::optional<uint16_t> free(std::uint16_t location);
  // std::optional<uint16_t> free(std::uint16_t location, std::size_t size);
};

#endif  // ALLOCATOR_H
