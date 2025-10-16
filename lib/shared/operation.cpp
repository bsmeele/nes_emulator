#include "operation.h"

#include <unordered_map>
#include <stdexcept>

bool OperationUtil::is_operation(Operation op) {
  switch (op) {
    case Operation::ADC:
    case Operation::AND:
    case Operation::ASL:
    case Operation::BCC:
    case Operation::BCS:
    case Operation::BEQ:
    case Operation::BIT:
    case Operation::BMI:
    case Operation::BNE:
    case Operation::BPL:
    case Operation::BRK:
    case Operation::BVC:
    case Operation::BVS:
    case Operation::CLC:
    case Operation::CLD:
    case Operation::CLI:
    case Operation::CLV:
    case Operation::CMP:
    case Operation::CPX:
    case Operation::CPY:
    case Operation::DEC:
    case Operation::DEX:
    case Operation::DEY:
    case Operation::EOR:
    case Operation::INC:
    case Operation::INX:
    case Operation::INY:
    case Operation::JMP:
    case Operation::JSR:
    case Operation::LDA:
    case Operation::LDX:
    case Operation::LDY:
    case Operation::LSR:
    case Operation::NOP:
    case Operation::ORA:
    case Operation::PHA:
    case Operation::PHP:
    case Operation::PLA:
    case Operation::PLP:
    case Operation::ROL:
    case Operation::ROR:
    case Operation::RTI:
    case Operation::RTS:
    case Operation::SBC:
    case Operation::SEC:
    case Operation::SED:
    case Operation::SEI:
    case Operation::STA:
    case Operation::STX:
    case Operation::STY:
    case Operation::TAX:
    case Operation::TAY:
    case Operation::TSX:
    case Operation::TXA:
    case Operation::TXS:
    case Operation::TYA:
      return true;
    default:
      return false;
  }
}

bool OperationUtil::is_branch(Operation op) {
  switch (op) {
    case Operation::BCC:
    case Operation::BCS:
    case Operation::BEQ:
    case Operation::BMI:
    case Operation::BNE:
    case Operation::BPL:
    case Operation::BVC:
    case Operation::BVS:
      return true;
    default:
      return false;
  }
}

std::string OperationUtil::to_string(Operation op) {
  switch (op) {
    case Operation::ADC:
      return "ADC";
    case Operation::AND:
      return "AND";
    case Operation::ASL:
      return "ASL";
    case Operation::BCC:
      return "BCC";
    case Operation::BCS:
      return "BCS";
    case Operation::BEQ:
      return "BEQ";
    case Operation::BIT:
      return "BIT";
    case Operation::BMI:
      return "BMI";
    case Operation::BNE:
      return "BNE";
    case Operation::BPL:
      return "BPL";
    case Operation::BRK:
      return "BRK";
    case Operation::BVC:
      return "BVC";
    case Operation::BVS:
      return "BVS";
    case Operation::CLC:
      return "CLC";
    case Operation::CLD:
      return "CLD";
    case Operation::CLI:
      return "CLI";
    case Operation::CLV:
      return "CLV";
    case Operation::CMP:
      return "CMP";
    case Operation::CPX:
      return "CPX";
    case Operation::CPY:
      return "CPY";
    case Operation::DEC:
      return "DEC";
    case Operation::DEX:
      return "DEX";
    case Operation::DEY:
      return "DEY";
    case Operation::EOR:
      return "EOR";
    case Operation::INC:
      return "INC";
    case Operation::INX:
      return "INX";
    case Operation::INY:
      return "INY";
    case Operation::JMP:
      return "JMP";
    case Operation::JSR:
      return "JSR";
    case Operation::LDA:
      return "LDA";
    case Operation::LDX:
      return "LDX";
    case Operation::LDY:
      return "LDY";
    case Operation::LSR:
      return "LSR";
    case Operation::NOP:
      return "NOP";
    case Operation::ORA:
      return "ORA";
    case Operation::PHA:
      return "PHA";
    case Operation::PHP:
      return "PHP";
    case Operation::PLA:
      return "PLA";
    case Operation::PLP:
      return "PLP";
    case Operation::ROL:
      return "ROL";
    case Operation::ROR:
      return "ROR";
    case Operation::RTI:
      return "RTI";
    case Operation::RTS:
      return "RTS";
    case Operation::SBC:
      return "SBC";
    case Operation::SEC:
      return "SEC";
    case Operation::SED:
      return "SED";
    case Operation::SEI:
      return "SEI";
    case Operation::STA:
      return "STA";
    case Operation::STX:
      return "STX";
    case Operation::STY:
      return "STY";
    case Operation::TAX:
      return "TAX";
    case Operation::TAY:
      return "TAY";
    case Operation::TSX:
      return "TSX";
    case Operation::TXA:
      return "TXA";
    case Operation::TXS:
      return "TXS";
    case Operation::TYA:
      return "TYA";
    default:
      return "NotSupported";
  }
}

const std::unordered_map<std::string, Operation> operation_parse = {
  {"ADC", Operation::ADC},
  {"AND", Operation::AND},
  {"ALS", Operation::ASL},
  {"BCC", Operation::BCC},
  {"BCS", Operation::BCS},
  {"BEQ", Operation::BEQ},
  {"BIT", Operation::BIT},
  {"BMI", Operation::BMI},
  {"BNE", Operation::BNE},
  {"BPL", Operation::BPL},
  {"BRK", Operation::BRK},
  {"BVC", Operation::BVC},
  {"BVS", Operation::BVS},
  {"CLC", Operation::CLC},
  {"CLD", Operation::CLD},
  {"CLI", Operation::CLI},
  {"CLV", Operation::CLV},
  {"CMP", Operation::CMP},
  {"CPX", Operation::CPX},
  {"CPY", Operation::CPY},
  {"DEC", Operation::DEC},
  {"DEX", Operation::DEX},
  {"DEY", Operation::DEY},
  {"EOR", Operation::EOR},
  {"INC", Operation::INC},
  {"INX", Operation::INX},
  {"INY", Operation::INY},
  {"JMP", Operation::JMP},
  {"LSR", Operation::JSR},
  {"LDA", Operation::LDA},
  {"LDX", Operation::LDX},
  {"LDY", Operation::LDY},
  {"LSR", Operation::LSR},
  {"NOP", Operation::NOP},
  {"ORA", Operation::ORA},
  {"PHA", Operation::PHA},
  {"PHP", Operation::PHP},
  {"PLA", Operation::PLA},
  {"PLP", Operation::PLP},
  {"ROL", Operation::ROL},
  {"ROR", Operation::ROR},
  {"RTI", Operation::RTI},
  {"RTS", Operation::RTS},
  {"SBC", Operation::SBC},
  {"SEC", Operation::SEC},
  {"SED", Operation::SED},
  {"SEI", Operation::SEI},
  {"STA", Operation::STA},
  {"STX", Operation::STX},
  {"STY", Operation::STY},
  {"TAX", Operation::TAX},
  {"TAY", Operation::TAY},
  {"TSX", Operation::TSX},
  {"TXA", Operation::TXA},
  {"TXS", Operation::TXS},
  {"TYA", Operation::TYA}
};

Operation OperationUtil::from_string(std::string op_str) {
  try {
    Operation op = operation_parse.at(op_str);
    return op;
  } catch (std::out_of_range) {
    return Operation::NotSupported;
  }
}
