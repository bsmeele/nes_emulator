#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "ast.h"
#include "allocator.h"

#include "shared/access_modifiers.h"

class Assembler {
private:
TEST_PUBLIC:
  AST ast;
  Allocator allocator;

public:
  Assembler();

  std::vector<uint8_t> assemble(std::stringstream& source);

  void build_ast(std::stringstream& asm_src);
  void resolve_directives();
  void resolve_label_ambiguity();
  void allocate_variables();
  void resolve_variable_operands();
  void resolve_address_mode_ambiguity();
  void allocate_codeblocks();
  void resolve_labels();
  std::vector<uint8_t> generate_machine_code();
};

#endif  //ASSEMBLER_H
