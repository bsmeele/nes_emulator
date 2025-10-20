#ifndef ADDRESS_MODE_H
#define ADDRESS_MODE_H

#include <cstdint>
#include <string>
#include <optional>

enum class AddressMode: uint8_t {
  Absolute,
  AbsoluteX,
  AbsoluteY,
  Accumulator,  // Might be behave identically to implicit
  Immediate,
  Implied,
  Indirect,
  IndirectX,
  IndirectY,
  Relative,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,

  AmbiguousZeropageOrAbsoluteOrRelative,
  AmbiguousZeropageXOrAbsoluteX,
  AmbiguousZeropageYOrAbsoluteY,
  NotSupported,
};

namespace AddressModeUtil {
  bool is_address_mode(AddressMode addr_mode);
  std::string to_string(AddressMode addr_mode);
  std::optional<size_t> get_size(AddressMode addr_mode);
}

#endif  // ADDRESS_MODE_H
