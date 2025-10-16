#ifndef AST_H
#define AST_H

#include "shared/address_mode.h"
#include "shared/operation.h"
#include "shared/access_modifiers.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <cstdint>
#include <variant>

struct Instruction {
  AddressMode address_mode;
  Operation operation;
  std::variant<uint16_t, std::string> operand;
};

struct CodeBlock {
  std::vector<Instruction> instr_list;
};

class AST {
private:
TEST_PUBLIC:
  // std::vector<CodeBlock> block_list;
  std::vector<Instruction> instr_list;
  std::unordered_map<std::string, uint16_t> symbol_map;
  std::unordered_set<std::string> unresolved_symbols;
public:
  AST(std::stringstream& asm_src);

  void print();
};

#endif  // AST_H
