#include "cpu.h"

std::optional<uint16_t> CPU::addressing_absolute() {
  this->lockout_counter += 2;

  uint8_t low_byte = this->bus->read(this->program_counter);
  uint8_t high_byte = this->bus->read(this->program_counter + 1);
  this->program_counter += 2;

  return (high_byte << 8) | low_byte;
}

std::optional<uint16_t> CPU::addressing_absoluteX() {
  this->lockout_counter += 2;

  uint8_t low_byte = this->bus->read(this->program_counter);
  uint8_t high_byte = this->bus->read(this->program_counter + 1);
  this->program_counter += 2;

  uint16_t address = low_byte + x_index;
  if (address > 0xFF) {
    // Page boundary crossed
    this->lockout_counter += 1;
  }
  address += high_byte << 8;

  return address;
}

std::optional<uint16_t> CPU::addressing_absoluteY() {
  this->lockout_counter += 2;

  uint8_t low_byte = this->bus->read(this->program_counter);
  uint8_t high_byte = this->bus->read(this->program_counter + 1);
  this->program_counter += 2;

  uint16_t address = low_byte + y_index;
  if (address > 0xFF) {
    // Page boundary crossed
    this->lockout_counter += 1;
  }
  address += high_byte << 8;

  return address;
}

std::optional<uint16_t> CPU::addressing_accumulator() {
  return {};  // Accumulator as the operand is implied
}

std::optional<uint16_t> CPU::addressing_immediate() {
  uint16_t address = this->program_counter;
  this->program_counter += 1;
  
  return address;
}

std::optional<uint16_t> CPU::addressing_implied() {
  return {};
}

std::optional<uint16_t> CPU::addressing_indirect() {
  this->lockout_counter += 4;

  uint8_t low_byte = this->bus->read(this->program_counter);
  uint8_t high_byte = this->bus->read(this->program_counter + 1);
  this->program_counter += 2;

  uint16_t address = (high_byte << 8) | low_byte;
  low_byte = this->bus->read(address);
  high_byte = this->bus->read(address + 1);

  address = (high_byte << 8) | low_byte;
  return address;
}

std::optional<uint16_t> CPU::addressing_indirectX() {
  this->lockout_counter += 4;

  uint8_t low_byte = this->bus->read(this->program_counter);
  this->program_counter += 1;

  low_byte += this->x_index;  // Addition of the x register will not cause page boundaries to be crossed

  uint16_t address = low_byte;
  low_byte = this->bus->read(address);
  uint8_t high_byte = this->bus->read(address + 1);

  address = (high_byte << 8) | low_byte;
  return address;
}

std::optional<uint16_t> CPU::addressing_indirectY() {
  this->lockout_counter += 3;

  uint8_t low_byte = this->bus->read(this->program_counter);
  this->program_counter += 1;

  uint16_t address = low_byte;
  low_byte = this->bus->read(address);
  uint8_t high_byte = this->bus->read(address + 1);

  address = low_byte + y_index;
  if (address > 0xFF) {
    // Page boundary crossed
    this->lockout_counter += 1;
  }
  address += high_byte << 8;

  return address;
}

std::optional<uint16_t> CPU::addressing_relative() {
  uint8_t address = this->program_counter;
  program_counter += 1;
  return address;
}

std::optional<uint16_t> CPU::addressing_zeropage() {
  this->lockout_counter += 1;

  uint8_t low_byte = this->bus->read(this->program_counter);
  this->program_counter += 1;

  uint16_t address = low_byte;
  return address;
}

std::optional<uint16_t> CPU::addressing_zeropageX() {
  this->lockout_counter += 2;

  uint8_t low_byte = this->bus->read(this->program_counter);
  this->program_counter += 1;

  low_byte += this->x_index;
  uint16_t address = low_byte;
  return address;
}

std::optional<uint16_t> CPU::addressing_zeropageY() {
  this->lockout_counter += 2;

  uint8_t low_byte = this->bus->read(this->program_counter);
  this->program_counter += 1;

  low_byte += this->y_index;
  uint16_t address = low_byte;
  return address;
}
