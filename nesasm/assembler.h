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

  void build_ast(std::stringstream& asm_src);
  void resolve_directives();
  void allocate_variables();
  void resolve_variable_operands();
  void resolve_ambiguity();
  void allocate_codeblocks();
  void resolve_labels();
  std::vector<uint8_t> generate_machine_code();
};

#endif  //ASSEMBLER_H
