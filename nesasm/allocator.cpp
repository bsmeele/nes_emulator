#include "allocator.h"

Allocator::Allocator() {
  this->memory_blocks.push_back(std::pair<uint16_t, uint16_t>(0x0000, 0x00FF));  // First page
  // Second page is reserved for the stack
  this->memory_blocks.push_back(std::pair<uint16_t, uint16_t>(0x0200, 0x07FF));  // Remaining pages
}

std::optional<uint16_t> Allocator::reserve_block(size_t size) {
  // Find the first free block of the supplied size
  for (auto& range : this->memory_blocks) {
    if (range.second - range.first + 1 >= size) {
      return this->reserve_at(range.first, size);
    }
  }
  
  // No space for requested size
  return {};
}

std::optional<uint16_t> Allocator::reserve_at(uint16_t location) {
  return this->reserve_at(location, 1);
}

std::optional<uint16_t> Allocator::reserve_at(uint16_t location, size_t size) {
  for (int i = 0; i < this->memory_blocks.size(); i++) {
    std::pair<uint16_t, uint16_t>& range = this->memory_blocks[i];

    if (location < range.first
      || location > range.second) {
      // Range does not contain the requested location, skipping to next range
      // Requested size does not fit in the range, skipping to next range
      continue;
    }

    if (range.second - range.first + 1 < size) {
      // requested size does not fit at that location
      return {};
    }

    if (location == range.first) {  // Location starts at the start of a range
      if (range.second - range.first + 1 == size) {  // Requested size is equal to the range size
        // Delete range from the list
        this->memory_blocks.erase(this->memory_blocks.begin() + i);

      } else {  // Range is larger thatn the requested size
        // Shorten the range
        range.first += size;
      }

    } else {  // Location starts in the middle of a range
      std::pair<uint16_t, uint16_t> range1 = std::pair(range.first, location - 1);

      if (location + size < range.second) {  // Requested size starts and ends in the middle of a range
        // Split the range
        std::pair<uint16_t, uint16_t> range2 = std::pair(location + size, range.second);
        this->memory_blocks.insert(this->memory_blocks.begin() + i + 1, range2);
      }
      this->memory_blocks[i] = range1;
    }

    return location;
  }

  // Requested memory location is not available
  return {};
}
