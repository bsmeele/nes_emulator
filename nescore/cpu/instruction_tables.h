#ifndef INSTRUCTION_TABLES_H
#define INSTRUCTION_TABLES_H

#include "cpu/cpu.h"
#include "shared/address_mode.h"
#include "shared/operation.h"
#include "shared/instruction_lookup.h"

#include <cstdint>
#include <utility>

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
