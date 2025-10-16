#ifndef OPERATION_H
#define OPERATION_H

#include <cstdint>
#include <string>

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

namespace OperationUtil {
  bool is_operation(Operation op);

  bool is_branch(Operation op);

  std::string to_string(Operation op);

  Operation from_string(std::string op);
}

#endif  // OPERATION_H
