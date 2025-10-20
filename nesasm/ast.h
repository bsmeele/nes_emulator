#ifndef AST_H
#define AST_H

#include "directive.h"

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
#include <optional>

struct Instruction {
  AddressMode address_mode;
  Operation operation;
  std::variant<uint16_t, std::string> operand;
  std::optional<uint16_t> location;
  size_t line;
};

struct CodeBlock {
  std::string name;
  std::string entry;
  std::vector<Instruction> instr_list;
};

struct AST {
  std::vector<CodeBlock> block_list;
  std::vector<Instruction> instr_list;
  std::unordered_map<std::string, size_t> label_map;
  std::unordered_map<std::string, uint16_t> variable_map;
  std::unordered_set<std::string> unresolved_symbols;
  std::vector<Directive> unresolevd_directives;

  AST() = default;
  AST(std::stringstream& asm_src);
  void print();
};

#endif  // AST_H
