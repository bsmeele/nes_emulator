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
  void resole_symbols();
};

#endif  //ASSEMBLER_H
