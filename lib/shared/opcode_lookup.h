#ifndef OPCODE_LOOKUP_H
#define OPCODE_LOOKUP_H

#include "address_mode.h"
#include "operation.h"

#include <utility>
#include <unordered_map>

struct KeyHash {
  std::size_t operator()(const std::pair<AddressMode, Operation>& k) const {
    return static_cast<std::size_t>(static_cast<int>(k.first)) * 97 + static_cast<int>(k.second);
  }
};

const std::unordered_map<std::pair<AddressMode, Operation>, uint8_t, KeyHash> opcode_lookup = {
  // 0x0-
  {{AddressMode::Implied, Operation::BRK}, 0x00},
  {{AddressMode::IndirectX, Operation::ORA}, 0x01},
  {{AddressMode::ZeroPage, Operation::ORA}, 0x05},
  {{AddressMode::ZeroPage, Operation::ASL}, 0x06},
  {{AddressMode::Implied, Operation::PHP}, 0x08},
  {{AddressMode::Immediate, Operation::ORA}, 0x09},
  {{AddressMode::Accumulator, Operation::ASL}, 0x0A},
  {{AddressMode::Absolute, Operation::ORA}, 0x0D},
  {{AddressMode::Absolute, Operation::ASL}, 0x0E},

  // 0x1-
  {{AddressMode::Relative, Operation::BPL}, 0x10},
  {{AddressMode::IndirectY, Operation::ORA}, 0x11},
  {{AddressMode::ZeroPageX, Operation::ORA}, 0x15},
  {{AddressMode::ZeroPageX, Operation::ASL}, 0x16},
  {{AddressMode::Implied, Operation::CLC}, 0x18},
  {{AddressMode::AbsoluteY, Operation::ORA}, 0x19},
  {{AddressMode::AbsoluteX, Operation::ORA}, 0x1D},
  {{AddressMode::AbsoluteX, Operation::ASL}, 0x1E},

  // 0x2-
  {{AddressMode::Absolute, Operation::JSR}, 0x20},
  {{AddressMode::IndirectX, Operation::AND}, 0x21},
  {{AddressMode::ZeroPage, Operation::BIT}, 0x24},
  {{AddressMode::ZeroPage, Operation::AND}, 0x25},
  {{AddressMode::ZeroPage, Operation::ROL}, 0x26},
  {{AddressMode::Implied, Operation::PLP}, 0x28},
  {{AddressMode::Immediate, Operation::AND}, 0x29},
  {{AddressMode::Accumulator, Operation::ROL}, 0x2A},
  {{AddressMode::Absolute, Operation::BIT}, 0x2C},
  {{AddressMode::Absolute, Operation::AND}, 0x2D},
  {{AddressMode::Absolute, Operation::ROL}, 0x2E},

  // 0x3-
  {{AddressMode::Relative, Operation::BMI}, 0x30},
  {{AddressMode::IndirectY, Operation::AND}, 0x31},
  {{AddressMode::IndirectY, Operation::AND}, 0x32},
  {{AddressMode::ZeroPageX, Operation::AND}, 0x35},
  {{AddressMode::ZeroPageX, Operation::ROL}, 0x36},
  {{AddressMode::Implied, Operation::SEC}, 0x38},
  {{AddressMode::AbsoluteY, Operation::AND}, 0x39},
  {{AddressMode::AbsoluteX, Operation::AND}, 0x3D},
  {{AddressMode::AbsoluteX, Operation::ROL}, 0x3E},

  // 0x4-
  {{AddressMode::Implied, Operation::RTI}, 0x40},
  {{AddressMode::IndirectX, Operation::EOR}, 0x41},
  {{AddressMode::ZeroPage, Operation::EOR}, 0x45},
  {{AddressMode::ZeroPage, Operation::LSR}, 0x46},
  {{AddressMode::Implied, Operation::PHA}, 0x48},
  {{AddressMode::Immediate, Operation::EOR}, 0x49},
  {{AddressMode::Accumulator, Operation::LSR}, 0x4A},
  {{AddressMode::Absolute, Operation::JMP}, 0x4C},
  {{AddressMode::Absolute, Operation::EOR}, 0x4D},
  {{AddressMode::Absolute, Operation::LSR}, 0x4E},

  // 0x5-
  {{AddressMode::Relative, Operation::BVC}, 0x50},
  {{AddressMode::IndirectY, Operation::EOR}, 0x51},
  {{AddressMode::ZeroPageX, Operation::EOR}, 0x55},
  {{AddressMode::ZeroPageX, Operation::LSR}, 0x56},
  {{AddressMode::Implied, Operation::CLI}, 0x58},
  {{AddressMode::AbsoluteY, Operation::EOR}, 0x59},
  {{AddressMode::AbsoluteX, Operation::EOR}, 0x5D},
  {{AddressMode::AbsoluteX, Operation::LSR}, 0x5E},

  // 0x6-
  {{AddressMode::Implied, Operation::RTS}, 0x60},
  {{AddressMode::IndirectX, Operation::ADC}, 0x61},
  {{AddressMode::ZeroPage, Operation::ADC}, 0X65},
  {{AddressMode::ZeroPage, Operation::ROR}, 0x66},
  {{AddressMode::Implied, Operation::PLA}, 0x68},
  {{AddressMode::Immediate, Operation::ADC}, 0x69},
  {{AddressMode::Accumulator, Operation::ROR}, 0x6A},
  {{AddressMode::Indirect, Operation::JMP}, 0x6C},
  {{AddressMode::Absolute, Operation::ADC}, 0x6D},
  {{AddressMode::Absolute, Operation::ROR}, 0x6E},

  // 0x7-
  {{AddressMode::Relative, Operation::BVS}, 0x70},
  {{AddressMode::IndirectY, Operation::ADC}, 0x71},
  {{AddressMode::ZeroPageX, Operation::ADC}, 0x75},
  {{AddressMode::ZeroPageX, Operation::ROR}, 0x76},
  {{AddressMode::Implied, Operation::SEI}, 0x78},
  {{AddressMode::AbsoluteY, Operation::ADC}, 0x79},
  {{AddressMode::AbsoluteX, Operation::ADC}, 0x7D},
  {{AddressMode::AbsoluteX, Operation::ROR}, 0x7E},

  // 0x8-
  {{AddressMode::IndirectX, Operation::STA}, 0x81},
  {{AddressMode::ZeroPage, Operation::STY}, 0x84},
  {{AddressMode::ZeroPage, Operation::STA}, 0x85},
  {{AddressMode::ZeroPage, Operation::STX}, 0x86},
  {{AddressMode::Implied, Operation::DEY}, 0x88},
  {{AddressMode::Implied, Operation::TXA}, 0x8A},
  {{AddressMode::Absolute, Operation::STY}, 0x8C},
  {{AddressMode::Absolute, Operation::STA}, 0x8D},
  {{AddressMode::Absolute, Operation::STX}, 0x8E},

  // 0x9-
  {{AddressMode::Relative, Operation::BCC}, 0x90},
  {{AddressMode::IndirectY, Operation::STA}, 0x91},
  {{AddressMode::ZeroPageX, Operation::STY}, 0x94},
  {{AddressMode::ZeroPageX, Operation::STA}, 0x95},
  {{AddressMode::ZeroPageY, Operation::STX}, 0x96},
  {{AddressMode::Implied, Operation::TYA}, 0x98},
  {{AddressMode::AbsoluteY, Operation::STA}, 0x99},
  {{AddressMode::Implied, Operation::TXS}, 0x9A},
  {{AddressMode::AbsoluteX, Operation::STA}, 0x9D},

  // 0xA-
  {{AddressMode::Immediate, Operation::LDY}, 0xA0},
  {{AddressMode::IndirectX, Operation::LDA}, 0xA1},
  {{AddressMode::Immediate, Operation::LDX}, 0xA2},
  {{AddressMode::ZeroPage, Operation::LDY}, 0xA4},
  {{AddressMode::ZeroPage, Operation::LDA}, 0xA5},
  {{AddressMode::ZeroPage, Operation::LDX}, 0xA6},
  {{AddressMode::Implied, Operation::TAY}, 0xA8},
  {{AddressMode::Immediate, Operation::LDA}, 0xA9},
  {{AddressMode::Implied, Operation::TAX}, 0xAA},
  {{AddressMode::Absolute, Operation::LDY}, 0xAC},
  {{AddressMode::Absolute, Operation::LDA}, 0xAD},
  {{AddressMode::Absolute, Operation::LDX}, 0xAE},

  // 0xB-
  {{AddressMode::Relative, Operation::BCS}, 0xB0},
  {{AddressMode::IndirectY, Operation::LDA}, 0xB1},
  {{AddressMode::ZeroPageX, Operation::LDY}, 0xB4},
  {{AddressMode::ZeroPageX, Operation::LDA}, 0xB5},
  {{AddressMode::ZeroPageY, Operation::LDX}, 0xB6},
  {{AddressMode::Implied, Operation::CLV}, 0xB8},
  {{AddressMode::AbsoluteY, Operation::LDA}, 0xB9},
  {{AddressMode::Implied, Operation::TSX}, 0xBA},
  {{AddressMode::AbsoluteX, Operation::LDY}, 0xBC},
  {{AddressMode::AbsoluteX, Operation::LDA}, 0xBD},
  {{AddressMode::AbsoluteY, Operation::LDX}, 0xBE},

  // 0xC-
  {{AddressMode::Immediate, Operation::CPY}, 0xC0},
  {{AddressMode::IndirectX, Operation::CMP}, 0xC1},
  {{AddressMode::ZeroPage, Operation::CPY}, 0xC4},
  {{AddressMode::ZeroPage, Operation::CMP}, 0xC5},
  {{AddressMode::ZeroPage, Operation::DEC}, 0xC6},
  {{AddressMode::Implied, Operation::INY}, 0xC8},
  {{AddressMode::Immediate, Operation::CMP}, 0xC9},
  {{AddressMode::Implied, Operation::DEX}, 0xCA},
  {{AddressMode::Absolute, Operation::CPY}, 0xCC},
  {{AddressMode::Absolute, Operation::CMP}, 0xCD},
  {{AddressMode::Absolute, Operation::DEC}, 0xCE},

  // 0xD-
  {{AddressMode::Relative, Operation::BNE}, 0xD0},
  {{AddressMode::IndirectY, Operation::CMP}, 0xD1},
  {{AddressMode::ZeroPageX, Operation::CMP}, 0xD5},
  {{AddressMode::ZeroPageX, Operation::DEC}, 0xD6},
  {{AddressMode::Implied, Operation::CLD}, 0xD8},
  {{AddressMode::AbsoluteY, Operation::CMP}, 0xD9},
  {{AddressMode::AbsoluteX, Operation::CMP}, 0xDD},
  {{AddressMode::AbsoluteX, Operation::DEC}, 0xDE},
  
  // 0xE-
  {{AddressMode::Immediate, Operation::CPX}, 0xE0},
  {{AddressMode::IndirectX, Operation::SBC}, 0xE1},
  {{AddressMode::ZeroPage, Operation::CPX}, 0xE4},
  {{AddressMode::ZeroPage, Operation::SBC}, 0xE5},
  {{AddressMode::ZeroPage, Operation::INC}, 0xE6},
  {{AddressMode::Implied, Operation::INX}, 0xE8},
  {{AddressMode::Immediate, Operation::SBC}, 0xE9},
  {{AddressMode::Implied, Operation::NOP}, 0xEA},
  {{AddressMode::Absolute, Operation::CPX}, 0xEC},
  {{AddressMode::Absolute, Operation::SBC}, 0xED},
  {{AddressMode::Absolute, Operation::INC}, 0xEE},

  // 0xF-
  {{AddressMode::Relative, Operation::BEQ}, 0xF0},
  {{AddressMode::IndirectY, Operation::SBC}, 0xF1},
  {{AddressMode::ZeroPageX, Operation::SBC}, 0xF5},
  {{AddressMode::ZeroPageX, Operation::INC}, 0xF6},
  {{AddressMode::Implied, Operation::SED}, 0xF8},
  {{AddressMode::AbsoluteY, Operation::SBC}, 0xF9},
  {{AddressMode::AbsoluteX, Operation::SBC}, 0xFD},
  {{AddressMode::AbsoluteX, Operation::INC}, 0xFE},
};

#endif  // OPCODE_LOOKUP_H
