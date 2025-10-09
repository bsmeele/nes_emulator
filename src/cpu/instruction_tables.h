#ifndef INSTRUCTION_TABLES_H
#define INSTRUCTION_TABLES_H

#include "cpu/cpu.h"

#include <cstdint>
#include <utility>

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
  NotSupported
};

enum class Operation: uint8_t {
  ADC,  // And with carry
  AND,  // And (with accumulator)
  ASL,  // Arithmetic shift left
  BCC,  // Branch on carry clear
  BCS,  // Branch on carry set
  BEQ,  // Branch on equal (zero set)
  BIT,  // Bit test
  BMI,  // Branch on minus (negative set)
  BNE,  // Branch on not equal (zero clear)
  BPL,  // Branch on plus (negative clear)
  BRK,  // Break / interrupt
  BVC,  // Branch on overflow clear
  BVS,  // Branch on overflow set
  CLC,  // Clear carry
  CLD,  // Clear decimal
  CLI,  // Clear interrupt disable
  CLV,  // Clear overflow
  CMP,  // Compare (with accumulator)
  CPX,  // Compare with X
  CPY,  // Compare with Y
  DEC,  // Decrement
  DEX,  // Decrement X
  DEY,  // Decrement Y
  EOR,  // Exclusive or (with accumulator)
  INC,  // Increment
  INX,  // Increment X
  INY,  // Increment Y
  JMP,  // Jump
  JSR,  // Jump subroutine
  LDA,  // Load accumulator
  LDX,  // Load X
  LDY,  // Load Y
  LSR,  // Logical shift right
  NOP,  // No operation
  ORA,  // Or with accumulator
  PHA,  // Push accumulator
  PHP,  // Push processor status (SR)
  PLA,  // Pull accumulator
  PLP,  // Pull processor status (SR)
  ROL,  // Rotate left
  ROR,  // Rotate right
  RTI,  // Return from interrupt
  RTS,  // Return from subroutine
  SBC,  // Subtract with carry
  SEC,  // Set carry
  SED,  // Set decimal
  SEI,  // Set interrupt disable
  STA,  // Set accumulator
  STX,  // Store X
  STY,  // Store Y
  TAX,  // Transfer accumulator to X
  TAY,  // Transfer accumulator to Y
  TSX,  // Transfer stack ponter to X
  TXA,  // Transfer X to accumulator
  TXS,  // Transfer X to stack pointer
  TYA,  // Transfer Y to accumulator
  NotSupported,
};

const std::pair<AddressMode, Operation> instruction_lookup[256] = {
  // 0x0-
  {AddressMode::Implied, Operation::BRK},
  {AddressMode::IndirectX, Operation::ORA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::ORA},
  {AddressMode::ZeroPage, Operation::ASL},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::PHP},
  {AddressMode::Immediate, Operation::ORA},
  {AddressMode::Accumulator, Operation::ASL},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::ORA},
  {AddressMode::Absolute, Operation::ASL},
  {AddressMode::NotSupported, Operation::NotSupported},

  //0x1-
  {AddressMode::Relative, Operation::BPL},
  {AddressMode::IndirectY, Operation::ORA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::ORA},
  {AddressMode::ZeroPageX, Operation::ASL},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::CLC},
  {AddressMode::AbsoluteY, Operation::ORA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::ORA},
  {AddressMode::AbsoluteX, Operation::ASL},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x2-
  {AddressMode::Absolute, Operation::JSR},
  {AddressMode::IndirectX, Operation::AND},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::BIT},
  {AddressMode::ZeroPage, Operation::AND},
  {AddressMode::ZeroPage, Operation::ROL},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::PLP},
  {AddressMode::Immediate, Operation::AND},
  {AddressMode::Accumulator, Operation::ROL},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::BIT},
  {AddressMode::Absolute, Operation::AND},
  {AddressMode::Absolute, Operation::ROL},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x3-
  {AddressMode::Relative, Operation::BMI},
  {AddressMode::IndirectY, Operation::AND},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::AND},
  {AddressMode::ZeroPageX, Operation::ROL},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::SEC},
  {AddressMode::AbsoluteY, Operation::AND},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::AND},
  {AddressMode::AbsoluteX, Operation::ROL},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x4-
  {AddressMode::Implied, Operation::RTI},
  {AddressMode::IndirectX, Operation::AND},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::EOR},
  {AddressMode::ZeroPage, Operation::LSR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::PHA},
  {AddressMode::Immediate, Operation::EOR},
  {AddressMode::Accumulator, Operation::LSR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::JMP},
  {AddressMode::Absolute, Operation::EOR},
  {AddressMode::Absolute, Operation::LSR},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x5-
  {AddressMode::Relative, Operation::BVC},
  {AddressMode::IndirectY, Operation::EOR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::EOR},
  {AddressMode::ZeroPageX, Operation::LSR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::CLI},
  {AddressMode::AbsoluteY, Operation::EOR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::EOR},
  {AddressMode::AbsoluteX, Operation::LSR},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x6-
  {AddressMode::Implied, Operation::RTS},
  {AddressMode::IndirectX, Operation::ADC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::ADC},
  {AddressMode::ZeroPage, Operation::ROR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::PLA},
  {AddressMode::Immediate, Operation::ADC},
  {AddressMode::Accumulator, Operation::ROR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Indirect, Operation::JMP},
  {AddressMode::Absolute, Operation::ADC},
  {AddressMode::Absolute, Operation::ROR},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x7-
  {AddressMode::Relative, Operation::BVS},
  {AddressMode::IndirectY, Operation::ADC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::ADC},
  {AddressMode::ZeroPageX, Operation::ROR},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::SEI},
  {AddressMode::AbsoluteY, Operation::ADC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::ADC},
  {AddressMode::AbsoluteX, Operation::ROR},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x8-
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::IndirectX, Operation::STA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::STY},
  {AddressMode::ZeroPage, Operation::STA},
  {AddressMode::ZeroPage, Operation::STX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::DEY},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::TXA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::STY},
  {AddressMode::Absolute, Operation::STA},
  {AddressMode::Absolute, Operation::STX},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0x9-
  {AddressMode::Relative, Operation::BCC},
  {AddressMode::IndirectY, Operation::STA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::STY},
  {AddressMode::ZeroPageX, Operation::STA},
  {AddressMode::ZeroPageY, Operation::STX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::TYA},
  {AddressMode::AbsoluteY, Operation::STA},
  {AddressMode::Implied, Operation::TXS},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::STA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0xA-
  {AddressMode::Immediate, Operation::LDY},
  {AddressMode::IndirectX, Operation::LDA},
  {AddressMode::Immediate, Operation::LDX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::LDY},
  {AddressMode::ZeroPage, Operation::LDA},
  {AddressMode::ZeroPage, Operation::LDX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::TAY},
  {AddressMode::Immediate, Operation::LDA},
  {AddressMode::Implied, Operation::TAX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::LDY},
  {AddressMode::Absolute, Operation::LDA},
  {AddressMode::Absolute, Operation::LDX},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0xB-
  {AddressMode::Relative, Operation::BCS},
  {AddressMode::IndirectY, Operation::LDA},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::LDY},
  {AddressMode::ZeroPageX, Operation::LDA},
  {AddressMode::ZeroPageY, Operation::LDX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::CLV},
  {AddressMode::AbsoluteY, Operation::LDA},
  {AddressMode::Implied, Operation::TSX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::LDY},
  {AddressMode::AbsoluteX, Operation::LDA},
  {AddressMode::AbsoluteY, Operation::LDX},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0xC-
  {AddressMode::Immediate, Operation::CPY},
  {AddressMode::IndirectX, Operation::CMP},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::CPY},
  {AddressMode::ZeroPage, Operation::CMP},
  {AddressMode::ZeroPage, Operation::DEC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::INY},
  {AddressMode::Immediate, Operation::CMP},
  {AddressMode::Implied, Operation::DEX},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::CPY},
  {AddressMode::Absolute, Operation::CMP},
  {AddressMode::Absolute, Operation::DEC},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0xD-
  {AddressMode::Relative, Operation::BNE},
  {AddressMode::IndirectY, Operation::CMP},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::CMP},
  {AddressMode::ZeroPageX, Operation::DEC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::CLD},
  {AddressMode::AbsoluteY, Operation::CMP},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::CMP},
  {AddressMode::AbsoluteX, Operation::DEC},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0xE-
  {AddressMode::Immediate, Operation::CPX},
  {AddressMode::IndirectX, Operation::SBC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPage, Operation::CPX},
  {AddressMode::ZeroPage, Operation::SBC},
  {AddressMode::ZeroPage, Operation::INC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::INX},
  {AddressMode::Immediate, Operation::SBC},
  {AddressMode::Implied, Operation::NOP},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Absolute, Operation::CPX},
  {AddressMode::Absolute, Operation::SBC},
  {AddressMode::Absolute, Operation::INC},
  {AddressMode::NotSupported, Operation::NotSupported},

  // 0xF-
  {AddressMode::Relative, Operation::BEQ},
  {AddressMode::IndirectY, Operation::SBC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::ZeroPageX, Operation::SBC},
  {AddressMode::ZeroPageX, Operation::INC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::Implied, Operation::SED},
  {AddressMode::AbsoluteY, Operation::SBC},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::NotSupported, Operation::NotSupported},
  {AddressMode::AbsoluteX, Operation::SBC},
  {AddressMode::AbsoluteX, Operation::INC},
  {AddressMode::NotSupported, Operation::NotSupported},
};


std::optional<uint16_t> (CPU::*address_mode_lookup[13])() = {
  &CPU::addressing_absolute,
  &CPU::addressing_absoluteX,
  &CPU::addressing_absoluteY,
  &CPU::addressing_accumulator,
  &CPU::addressing_immediate,
  &CPU::addressing_implied,
  &CPU::addressing_indirect,
  &CPU::addressing_indirectX,
  &CPU::addressing_indirectY,
  &CPU::addressing_relative,
  &CPU::addressing_zeropage,
  &CPU::addressing_zeropageX,
  &CPU::addressing_zeropageY,
};

void (CPU::*operation_lookup[56])(std::optional<uint16_t>) = {
&CPU::ADC,
&CPU::AND,
&CPU::ASL,
&CPU::BCC,
&CPU::BCS,
&CPU::BEQ,
&CPU::BIT,
&CPU::BMI,
&CPU::BNE,
&CPU::BPL,
&CPU::BRK,
&CPU::BVC,
&CPU::BVS,
&CPU::CLC,
&CPU::CLD,
&CPU::CLI,
&CPU::CLV,
&CPU::CMP,
&CPU::CPX,
&CPU::CPY,
&CPU::DEC,
&CPU::DEX,
&CPU::DEY,
&CPU::EOR,
&CPU::INC,
&CPU::INX,
&CPU::INY,
&CPU::JMP,
&CPU::JSR,
&CPU::LDA,
&CPU::LDX,
&CPU::LDY,
&CPU::LSR,
&CPU::NOP,
&CPU::ORA,
&CPU::PHA,
&CPU::PHP,
&CPU::PLA,
&CPU::PLP,
&CPU::ROL,
&CPU::ROR,
&CPU::RTI,
&CPU::RTS,
&CPU::SBC,
&CPU::SEC,
&CPU::SED,
&CPU::SEI,
&CPU::STA,
&CPU::STX,
&CPU::STY,
&CPU::TAX,
&CPU::TAY,
&CPU::TSX,
&CPU::TXA,
&CPU::TXS,
&CPU::TYA,
};

#endif  // INSTRUCTION_TABLES_H
