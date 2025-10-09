#include "cpu/cpu.h"
#include "cpu/instruction_tables.h"

#include <stdexcept>

CPU::CPU(Bus* bus) {
  this->bus = bus;

  this->accumulator = 0x00;
  this->x_index = 0x00;
  this->y_index = 0x00;
  this->flag = 0b00000100;  // Unsure about the state of the B flag on power-up and reset, however it shouldn't matter since the B flag is only set when pushing the flag register to the stack
  this->stack_pointer = 0xFD;  // The stack pointer is reduced by 3 on reset. Since it defaults to $00, substracting by 3 results in $FD
  this->program_counter = 0xFFFC;
  
  this->lockout_counter = 0;
}
CPU::~CPU() {};

void CPU::reset() {
  // The accumulator, X-index, and Y-index are unchanged during reset
  this->flag |= 0b00000100;  // Everything except the I flag stays unchanged during reset. For the B flag see the commend in the constructor
  this->stack_pointer = this->stack_pointer - 3;  // Should correctly wrap around when underflow
  this->program_counter = 0xFFFC;
  
  this->lockout_counter = 0;
}

void CPU::tick() {
  if (this->lockout_counter > 0) {
    this->lockout_counter -= 1;
    return;
  }

  // Fetch instruction
  uint8_t opcode = this->bus->read(this->program_counter);
  this->program_counter += 1;

  // Decode instruction into address mode and operation
  auto [address_mode, operation] = instruction_lookup[opcode];
  if (address_mode == AddressMode::NotSupported) {
    throw std::runtime_error("Selected address mode not supported");
  }
  if (operation == Operation::NotSupported) {
    throw std::runtime_error("Selected operation not supported");
  }

  // Resolve address mode
  std::optional<uint16_t> address = (this->*address_mode_lookup[static_cast<uint8_t>(address_mode)])();

  // Resolve operation
  (this->*operation_lookup[static_cast<uint8_t>(operation)])(address);
};
