#include "cpu/cpu.h"

void CPU::ADC(std::optional<uint16_t> address) {  // Add with Carry
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  uint16_t result = this->accumulator + operand + (this->flag & 0x01);

  this->flag = (this->flag & ~0x01) | ((result > 0xFF) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | (((result & 0x00FF) == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x40) | ((((result ^ this->accumulator) & (result ^ operand) & 0x80) == 0x80) * 0x40);  // Overflow flag
  this->flag = (this->flag & ~0x80) | (result & 0x80);  // Negative flag

  this->accumulator = result;
}

void CPU::AND(std::optional<uint16_t> address) {  // Bitwise AND
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->accumulator &= operand;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::ASL(std::optional<uint16_t> address) {  // Arithmetic Shift Left
  this->lockout_counter += 1;
  
  uint8_t operand;
  if (address) {
    this->lockout_counter += 2;
    operand = this->bus->read(address.value());
  } else {
    operand = this->accumulator;
  }

  bool carry = (operand & 0x80) == 0x80;
  uint8_t result = operand << 1;

  this->flag = (this->flag & ~0x01) | (carry * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((result == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (result & 0x80);  // Negative flag

  if (address) {
    this->bus->write(address.value(), result);
  } else {
    this->accumulator = result;
  }
}

void CPU::BCC(std::optional<uint16_t> address) {  // Branch if Carry Clear
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x01) == 0x00) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BCS(std::optional<uint16_t> address) {  // Branch if Carry Set
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x01) == 0x01) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BEQ(std::optional<uint16_t> address) {  // Branch if Equal
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x02) == 0x02) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BIT(std::optional<uint16_t> address) {  // Bit Test
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x02) | (((operand & this->accumulator) == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0xC0) | (operand & 0xC0);  // Overflow and netagive flag
}

void CPU::BMI(std::optional<uint16_t> address) {  // Branch if Minus
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x80) == 0x80) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BNE(std::optional<uint16_t> address) {  // Branch if Not Equal
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x02) == 0x00) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BPL(std::optional<uint16_t> address) {  // Branch if Plus
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x80) == 0x00) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BRK(std::optional<uint16_t> address) {  // Break (software IRQ)
  this->lockout_counter += 6;

  this->program_counter += 1;

  this->bus->write(0x0100 | this->stack_pointer, (this->program_counter & 0xFF00) >> 8);  // Push upper PC
  this->bus->write(0x0100 | (this->stack_pointer - 1), this->program_counter & 0x00FF);   // Push lower PC
  this->bus->write(0x0100 | (this->stack_pointer - 2), this->flag | 0x10);                // Puch status register with B flag set
  this->stack_pointer -= 3;

  this->program_counter = 0xFFFE;
  this->flag = this->flag | 0x04;
}

void CPU::BVC(std::optional<uint16_t> address) {  // Branch if Overflow Clear
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x40) == 0x00) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BVS(std::optional<uint16_t> address) {  // Branch if Overflow Set
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if ((this->flag & 0x40) == 0x40) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if ((result & 0xFF00) != (this->program_counter & 0xFF00)) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::CLC(std::optional<uint16_t> address) {  // Clear Carry
  this->lockout_counter += 1;

  this->flag &= ~0x01;
}

void CPU::CLD(std::optional<uint16_t> address) {  // Clear Decimal
  this->lockout_counter += 1;

  this->flag &= ~0x08;
}

void CPU::CLI(std::optional<uint16_t> address) {  // Clear Interrupt Disable
  this->lockout_counter += 1;

  this->flag &= ~0x04;
}

void CPU::CLV(std::optional<uint16_t> address) {  // Clear Overflow
  this->lockout_counter += 1;

  this->flag &= ~0x40;
}

void CPU::CMP(std::optional<uint16_t> address) {  // Compare A
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x01) | ((this->accumulator >= operand) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((this->accumulator == operand) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((this->accumulator < operand) * 0x80);  // Negative flag
}

void CPU::CPX(std::optional<uint16_t> address) {  // Compare X
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x01) | ((this->x_index >= operand) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((this->x_index == operand) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((this->x_index < operand) * 0x80);  // Negative flag
}

void CPU::CPY(std::optional<uint16_t> address) {  // Compare Y
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x01) | ((this->y_index >= operand) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((this->y_index == operand) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((this->y_index < operand) * 0x80);  // Negative flag
}

void CPU::DEC(std::optional<uint16_t> address) {  // Decrement Memory
  this->lockout_counter += 3;

  uint8_t operand = this->bus->read(address.value());

  operand -= 1;

  this->flag = (this->flag & ~0x02) | ((operand == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (operand & 0x80);  // Negative flag

  this->bus->write(address.value(), operand);
}

void CPU::DEX(std::optional<uint16_t> address) {  // Decrement X
  this->lockout_counter += 1;

  this->x_index -= 1;

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::DEY(std::optional<uint16_t> address) {  // Decrement Y
  this->lockout_counter += 1;

  this->y_index -= 1;

  this->flag = (this->flag & ~0x02) | ((this->y_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->y_index & 0x80);  // Negative flag
}

void CPU::EOR(std::optional<uint16_t> address) {  // Bitwise Exclusive OR
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->accumulator ^= operand;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::INC(std::optional<uint16_t> address) {  // Increment Memory
  this->lockout_counter += 3;

  uint8_t operand = this->bus->read(address.value());

  operand += 1;

  this->flag = (this->flag & ~0x02) | ((operand == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (operand & 0x80);  // Negative flag

  this->bus->write(address.value(), operand);
}

void CPU::INX(std::optional<uint16_t> address) {  // Incremement X
  this->lockout_counter += 1;

  this->x_index += 1;

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::INY(std::optional<uint16_t> address) {  // Increment Y
  this->lockout_counter += 1;

  this->y_index += 1;

  this->flag = (this->flag & ~0x02) | ((this->y_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->y_index & 0x80);  // Negative flag
}

void CPU::JMP(std::optional<uint16_t> address) {  // Jump
  this->program_counter = address.value();
}

void CPU::JSR(std::optional<uint16_t> address) {  // Jump to Subroutine
  this->lockout_counter += 3;

  this->bus->write(0x0100 | this->stack_pointer, (this->program_counter & 0xFF00) >> 8);  // Push upper PC
  this->bus->write(0x0100 | (this->stack_pointer - 1), this->program_counter & 0x00FF);   // Push lower PC
  this->stack_pointer -= 2;

  this->program_counter = address.value();
}

void CPU::LDA(std::optional<uint16_t> address) {  // Load A
  this->lockout_counter += 1;

  this->accumulator = this->bus->read(address.value());

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::LDX(std::optional<uint16_t> address) {  // Load X
  this->lockout_counter += 1;

  this->x_index = this->bus->read(address.value());

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::LDY(std::optional<uint16_t> address) {  // Load Y
  this->lockout_counter += 1;

  this->y_index = this->bus->read(address.value());

  this->flag = (this->flag & ~0x02) | ((this->y_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->y_index & 0x80);  // Negative flag
}

void CPU::LSR(std::optional<uint16_t> address) {  // Logic Shift Right
  this->lockout_counter += 1;
  
  uint8_t operand;
  if (address) {
    this->lockout_counter += 2;
    operand = this->bus->read(address.value());
  } else {
    operand = this->accumulator;
  }

  bool carry = (operand & 0x01) == 0x01;
  uint8_t result = operand >> 1;

  this->flag = (this->flag & ~0x01) | (carry * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((result == 0x00) * 0x02);  // Zero flag
  this->flag = this->flag & ~0x80;  // Negative flag

  if (address) {
    this->bus->write(address.value(), result);
  } else {
    this->accumulator = result;
  }
}

void CPU::NOP(std::optional<uint16_t> address) {  // No Operation
  this->lockout_counter += 1;
}

void CPU::ORA(std::optional<uint16_t> address) {  // Bitwise OR
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->accumulator |= operand;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::PHA(std::optional<uint16_t> address) {  // Push A
  this->lockout_counter += 2;

  this->bus->write(0x0100 | this->stack_pointer, this->accumulator);
  this->stack_pointer -= 1;
}

void CPU::PHP(std::optional<uint16_t> address) {  // Push Processor Status
  this->lockout_counter += 2;

  this->bus->write(0x0100 | this->stack_pointer, this->flag | 0x20);
  this->stack_pointer -= 1;
}

void CPU::PLA(std::optional<uint16_t> address) {  // Pull A
  this->lockout_counter += 3;

  this->stack_pointer += 1;
  this->accumulator = this->bus->read(0x0100 | this->stack_pointer);

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::PLP(std::optional<uint16_t> address) {  // Pull Procesor Status
  this->lockout_counter += 3;

  this->stack_pointer += 1;
  this->flag = (this->flag & ~0xCF) | (this->bus->read(0x0100 | this->stack_pointer) & ~0x20);
}

void CPU::ROL(std::optional<uint16_t> address) {  // Rotate Left
  this->lockout_counter += 1;

  uint8_t operand;
  if (address) {
    this->lockout_counter += 2;
    operand = this->bus->read(address.value());
  } else {
    operand = this->accumulator;
  }

  bool carry = (operand & 0x80) == 0x80;
  uint8_t result = operand << 1;
  result |= this->flag & 0x01;

  this->flag = (this->flag & ~0x01) | (carry * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((result == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((result & 0x80) == 0x80) * 0x80;  // Negative flag

  if (address) {
    this->bus->write(address.value(), result);
  } else {
    this->accumulator = result;
  }
}

void CPU::ROR(std::optional<uint16_t> address) {  // Rotate Right
  this->lockout_counter += 1;

  uint8_t operand;
  if (address) {
    this->lockout_counter += 2;
    operand = this->bus->read(address.value());
  } else {
    operand = this->accumulator;
  }

  bool carry = (operand & 0x01) == 0x01;
  uint8_t result = operand >> 1;
  result |= ((this->flag & 0x01) == 0x01) * 0x80;

  this->flag = (this->flag & ~0x01) | (carry * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((result == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((result & 0x80) == 0x80) * 0x80;  // Negative flag

  if (address) {
    this->bus->write(address.value(), result);
  } else {
    this->accumulator = result;
  }
}

void CPU::RTI(std::optional<uint16_t> address) {  // Return from Interrupt
  this->lockout_counter += 5;

  this->flag = (this->flag & ~0xCF) | (this->bus->read(0x0100 | (this->stack_pointer + 1)) & 0xCF);
  this->program_counter = this->bus->read(0x0100 | (this->stack_pointer + 2));
  this->program_counter |= this->bus->read(0x0100 | (this->stack_pointer + 3)) << 8;
  this->stack_pointer += 3;
}

void CPU::RTS(std::optional<uint16_t> address) {  // Return from Subroutine
  this->lockout_counter += 5;

  this->program_counter = this->bus->read(0x0100 | (this->stack_pointer + 1));
  this->program_counter |= this->bus->read(0x0100 | (this->stack_pointer + 2)) << 8;
  this->stack_pointer += 2;
  
  this->program_counter += 1;
}

void CPU::SBC(std::optional<uint16_t> address) {  // Subtract with Carry
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  int16_t result = this->accumulator - operand - (~this->flag & 0x01);

  this->flag = (this->flag & ~0x01) | (~(result < 0x00) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | (((result & 0x00FF) == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x40) | ((((result ^ this->accumulator) & (result ^ ~operand) & 0x80) == 0x80) * 0x40);  // Overflow flag
  this->flag = (this->flag & ~0x80) | (result & 0x80);  // Negative flag

  this->accumulator = result;
}

void CPU::SEC(std::optional<uint16_t> address) {  // Set Carry
  this->lockout_counter += 1;

  this->flag |= 0x01;
}

void CPU::SED(std::optional<uint16_t> address) {  // Set Decimal
  this->lockout_counter += 1;

  this->flag |= 0x08;
}

void CPU::SEI(std::optional<uint16_t> address) {  // Set Interrupt Disable
  this->lockout_counter += 1;

  this->flag |= 0x04;
}

void CPU::STA(std::optional<uint16_t> address) {  // Store A
  this->lockout_counter += 1;

  this->bus->write(address.value(), this->accumulator);
}

void CPU::STX(std::optional<uint16_t> address) {  // Store X
  this->lockout_counter += 1;

  this->bus->write(address.value(), this->x_index);
}

void CPU::STY(std::optional<uint16_t> address) {  // Store Y
  this->lockout_counter += 1;

  this->bus->write(address.value(), this->y_index);
}

void CPU::TAX(std::optional<uint16_t> address) {  // Transer A to X
  this->lockout_counter += 1;

  this->x_index = this->accumulator;

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::TAY(std::optional<uint16_t> address) {  // Transer A to Y
  this->lockout_counter += 1;

  this->y_index = this->accumulator;

  this->flag = (this->flag & ~0x02) | ((this->y_index == 0) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->y_index & 0x80);  // Negative flag
}

void CPU::TSX(std::optional<uint16_t> address) {  // Transfer Stack Pointer to X
  this->lockout_counter += 1;

  this->x_index = this->stack_pointer;

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::TXA(std::optional<uint16_t> address) {  // Transer X to A
  this->lockout_counter += 1;

  this->accumulator = this->x_index;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::TXS(std::optional<uint16_t> address) {  // Transfer X to Stack Pointer
  this->lockout_counter += 1;

  this->stack_pointer = this->x_index;

  this->flag = (this->flag & ~0x02) | ((this->stack_pointer == 0) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->stack_pointer & 0x80);  // Negative flag
}

void CPU::TYA(std::optional<uint16_t> address) {  // Transer Y to A
  this->lockout_counter += 1;

  this->accumulator = this->y_index;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}
