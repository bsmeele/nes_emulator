#ifndef CPU_H
#define CPU_H

#include "bus.h"

#include <cstdint>
#include <optional>

class CPU {
private:
  Bus* bus;

  uint8_t accumulator;
  uint8_t x_index;
  uint8_t y_index;
  uint8_t flag;             // NO1BDIZC
  uint8_t stack_pointer;
  uint16_t program_counter;
  uint8_t lockout_counter;

public:
  CPU(Bus* bus);
  ~CPU();

  void reset();
  void tick();

  std::optional<uint16_t> addressing_absolute();
  std::optional<uint16_t> addressing_absoluteX();
  std::optional<uint16_t> addressing_absoluteY();
  std::optional<uint16_t> addressing_accumulator();
  std::optional<uint16_t> addressing_immediate();
  std::optional<uint16_t> addressing_implied();
  std::optional<uint16_t> addressing_indirect();
  std::optional<uint16_t> addressing_indirectX();
  std::optional<uint16_t> addressing_indirectY();
  std::optional<uint16_t> addressing_relative();
  std::optional<uint16_t> addressing_zeropage();
  std::optional<uint16_t> addressing_zeropageX();
  std::optional<uint16_t> addressing_zeropageY();

  void ADC(std::optional<uint16_t> address);
  void AND(std::optional<uint16_t> address);
  void ASL(std::optional<uint16_t> address);
  void BCC(std::optional<uint16_t> address);
  void BCS(std::optional<uint16_t> address);
  void BEQ(std::optional<uint16_t> address);
  void BIT(std::optional<uint16_t> address);
  void BMI(std::optional<uint16_t> address);
  void BNE(std::optional<uint16_t> address);
  void BPL(std::optional<uint16_t> address);
  void BRK(std::optional<uint16_t> address);
  void BVC(std::optional<uint16_t> address);
  void BVS(std::optional<uint16_t> address);
  void CLC(std::optional<uint16_t> address);
  void CLD(std::optional<uint16_t> address);
  void CLI(std::optional<uint16_t> address);
  void CLV(std::optional<uint16_t> address);
  void CMP(std::optional<uint16_t> address);
  void CPX(std::optional<uint16_t> address);
  void CPY(std::optional<uint16_t> address);
  void DEC(std::optional<uint16_t> address);
  void DEX(std::optional<uint16_t> address);
  void DEY(std::optional<uint16_t> address);
  void EOR(std::optional<uint16_t> address);
  void INC(std::optional<uint16_t> address);
  void INX(std::optional<uint16_t> address);
  void INY(std::optional<uint16_t> address);
  void JMP(std::optional<uint16_t> address);
  void JSR(std::optional<uint16_t> address);
  void LDA(std::optional<uint16_t> address);
  void LDX(std::optional<uint16_t> address);
  void LDY(std::optional<uint16_t> address);
  void LSR(std::optional<uint16_t> address);
  void NOP(std::optional<uint16_t> address);
  void ORA(std::optional<uint16_t> address);
  void PHA(std::optional<uint16_t> address);
  void PHP(std::optional<uint16_t> address);
  void PLA(std::optional<uint16_t> address);
  void PLP(std::optional<uint16_t> address);
  void ROL(std::optional<uint16_t> address);
  void ROR(std::optional<uint16_t> address);
  void RTI(std::optional<uint16_t> address);
  void RTS(std::optional<uint16_t> address);
  void SBC(std::optional<uint16_t> address);
  void SEC(std::optional<uint16_t> address);
  void SED(std::optional<uint16_t> address);
  void SEI(std::optional<uint16_t> address);
  void STA(std::optional<uint16_t> address);
  void STX(std::optional<uint16_t> address);
  void STY(std::optional<uint16_t> address);
  void TAX(std::optional<uint16_t> address);
  void TAY(std::optional<uint16_t> address);
  void TSX(std::optional<uint16_t> address);
  void TXA(std::optional<uint16_t> address);
  void TXS(std::optional<uint16_t> address);
  void TYA(std::optional<uint16_t> address);
};

#endif  // CPU_H
