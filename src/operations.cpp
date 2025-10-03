#include "cpu.h"

void CPU::ADC(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  uint16_t result = this->accumulator + operand + (this->flag & 0x01);

  this->flag = (this->flag & ~0x01) | ((result > 0xFF) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((result == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x40) | (((result ^ this->accumulator) & (result ^ operand) & 0x80) * 0x40);  // Overflow flag
  this->flag = (this->flag & ~0x80) | (result & 0x80);  // Negative flag

  this->accumulator = result;
}

void CPU::AND(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->accumulator &= operand;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::ASL(std::optional<uint16_t> address) {
  this->lockout_counter += 1;
  
  uint8_t operand;
  if (address) {
    this->lockout_counter += 2;
    operand = this->bus->read(address.value());
  } else {
    operand = this->accumulator;
  }

  uint16_t result = operand << 1;

  this->flag = (this->flag & ~0x01) | ((result > 0xFF) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((result == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (result &  0x80);  // Negative flag

  if (address) {
    this->bus->write(address.value(), result);
  } else {
    this->accumulator = result;
  }
}

void CPU::BCC(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x01 == 0x01) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BCS(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x01 != 0x01) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BEQ(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x02 == 0x02) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BIT(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x02) | ((operand & this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0xC0) | (operand & 0xC0);  // Overflow and netagive flag
}

void CPU::BMI(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x80 == 0x80) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BEQ(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x02 != 0x02) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BMI(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x80 != 0x80) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BRK(std::optional<uint16_t> address) {
  this->lockout_counter += 6;

  this->program_counter += 1;

  this->bus->write(0x0100 | this->stack_pointer, (this->program_counter & 0xFF00) >> 8);  // Push upper PC
  this->bus->write(0x0100 | (this->stack_pointer - 1), this->program_counter * 0x00FF);   // Push lower PC
  this->bus->write(0x0100 | (this->stack_pointer - 2), this->flag | 0x10);                // Puch status register with B flag set
  this->stack_pointer -= 3;

  this->program_counter = 0xFFFE;
  this->flag = this->flag | 0x04;
}

void CPU::BVC(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x40 != 0x40) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::BVS(std::optional<uint16_t> address) {
  this->lockout_counter =+ 1;

  uint8_t offset = this->bus->read(address.value());

  if (this->flag & 0x40 == 0x40) {
    this->lockout_counter += 1;

    uint16_t result = this->program_counter + static_cast<int8_t>(offset);

    if (result & 0x0100 != this->program_counter & 0x0100) {
      // Page boundary crossed
      this->lockout_counter += 1;
    }

    this->program_counter = result;
  }
}

void CPU::CLC(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->flag &= 0xFE;
}

void CPU::CLD(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->flag &= 0xF7;
}

void CPU::CLI(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->flag &= 0xFB;
}

void CPU::CLV(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->flag &= 0xBF;
}

void CPU::CMP(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x01) | ((this->accumulator >= operand) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((this->accumulator == operand) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((this->accumulator < operand) * 0x80);  // Negative flag
}

void CPU::CPX(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x01) | ((this->x_index >= operand) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((this->x_index == operand) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((this->x_index < operand) * 0x80);  // Negative flag
}

void CPU::CPY(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->flag = (this->flag & ~0x01) | ((this->y_index >= operand) * 0x01);  // Carry flag
  this->flag = (this->flag & ~0x02) | ((this->y_index == operand) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | ((this->y_index < operand) * 0x80);  // Negative flag
}

void CPU::DEC(std::optional<uint16_t> address) {
  this->lockout_counter += 3;

  uint8_t operand = this->bus->read(address.value());

  operand -= 1;

  this->flag = (this->flag & ~0x02) | ((operand == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (operand & 0x80);  // Negative flag

  this->bus->write(address.value(), operand);
}

void CPU::DEX(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->x_index -= 1;

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::DEY(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->y_index -= 1;

  this->flag = (this->flag & ~0x02) | ((this->y_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->y_index & 0x80);  // Negative flag
}

void CPU::EOR(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  uint8_t operand = this->bus->read(address.value());

  this->accumulator ^= operand;

  this->flag = (this->flag & ~0x02) | ((this->accumulator == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->accumulator & 0x80);  // Negative flag
}

void CPU::INC(std::optional<uint16_t> address) {
  this->lockout_counter += 3;

  uint8_t operand = this->bus->read(address.value());

  operand += 1;

  this->flag = (this->flag & ~0x02) | ((operand == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (operand & 0x80);  // Negative flag

  this->bus->write(address.value(), operand);
}

void CPU::INX(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->x_index += 1;

  this->flag = (this->flag & ~0x02) | ((this->x_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->x_index & 0x80);  // Negative flag
}

void CPU::INY(std::optional<uint16_t> address) {
  this->lockout_counter += 1;

  this->y_index += 1;

  this->flag = (this->flag & ~0x02) | ((this->y_index == 0x00) * 0x02);  // Zero flag
  this->flag = (this->flag & ~0x80) | (this->y_index & 0x80);  // Negative flag
}
