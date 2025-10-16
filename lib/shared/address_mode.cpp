#include "address_mode.h"

bool AddressModeUtil::is_address_mode(AddressMode addr_mode) {
  switch (addr_mode) {
    case AddressMode::Absolute:
    case AddressMode::AbsoluteX:
    case AddressMode::AbsoluteY:
    case AddressMode::Accumulator:
    case AddressMode::Immediate:
    case AddressMode::Implied:
    case AddressMode::Indirect:
    case AddressMode::IndirectX:
    case AddressMode::IndirectY:
    case AddressMode::Relative:
    case AddressMode::ZeroPage:
    case AddressMode::ZeroPageX:
    case AddressMode::ZeroPageY:
      return true;
    default:
      return false;
  }
}

std::string AddressModeUtil::to_string(AddressMode addr_mode) {
  switch (addr_mode) {
    case AddressMode::Absolute:
      return "Absolute";
    case AddressMode::AbsoluteX:
      return "AbsoluteX";
    case AddressMode::AbsoluteY:
      return "AbsoluteY";
    case AddressMode::Accumulator:
      return "Accumulator";
    case AddressMode::Immediate:
      return "Immediate";
    case AddressMode::Implied:
      return "Implied";
    case AddressMode::Indirect:
      return "Indirect";
    case AddressMode::IndirectX:
      return "IndirectX";
    case AddressMode::IndirectY:
      return "IndirectY";
    case AddressMode::Relative:
      return "Relative";
    case AddressMode::ZeroPage:
      return "ZeroPage";
    case AddressMode::ZeroPageX:
      return "ZeroPageX";
    case AddressMode::ZeroPageY:
      return "ZeroPageY";
    case AddressMode::AmbiguousZeropageOrAbsoluteOrRelative:
      return "AmbiguousZeropageOrAbsoluteOrRelative";
    case AddressMode::AmbiguousZeropageXOrAbsoluteX:
      return "AmbiguousZeropageXOrAbsoluteX";
    case AddressMode::AmbiguousZeropageYOrAbsoluteY:
      return "AmbiguousZeropageYOrAbsoluteY";
    case AddressMode::NotSupported:
      return "NotSupported";
    default:
      return "NotSupported";
  }
}

