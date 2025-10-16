#include "assembler.h"

#include <fstream>
#include <regex>
#include <string>

int main() {
  // std::string filename = "../rom_asm/fibonacci.asm";
  std::string filename = "../rom_asm/arithmetic.asm";
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  Assembler assembler = Assembler();
  assembler.build_ast(buffer);

#ifdef TESTING
  assembler.ast.print();
#endif

  // AST:
  //   List of blocks
  //     Instruction list
  //       Addressing mode
  //       Operation
  //       Operand
  //         Operand type
  //         Value
  //   Label map
  //   Variable set

  // Code generation:
  //   Assign memory locations to variables
  //   Place blocks
  //     Resolve labels
  //     Resolve variables

  // AST workflow:
  //   Read line
  //   Read first word
  //     If starts with `.`, resolve directive
  //     If ends in `:`, add to label hashmap
  //     If neither
  //       Get operation
  //       Get addressing mode
  //       Get operand

  return 0;
}
