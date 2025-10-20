#include "assembler.h"

#include <fstream>
#include <regex>
#include <string>
#include <iostream>

int main() {
  // std::string filename = "../rom_asm/fibonacci.asm";
  std::string filename = "../rom_asm/arithmetic.asm";
  // std::string filename = "../rom_asm/sorting.asm";
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  Assembler assembler = Assembler();
  assembler.build_ast(buffer);
  assembler.resolve_directives();
  assembler.allocate_variables();
  assembler.resolve_variable_operands();
  assembler.resolve_ambiguity();
  assembler.allocate_codeblocks();
  assembler.resolve_labels();

#ifdef TESTING
  assembler.ast.print();
#endif

  std::vector<uint8_t> code = assembler.generate_machine_code();

  for (auto byte : code) {
    std::cout << std::hex << std::uppercase << static_cast<int>(byte) << " ";
  }
  std::cout << std::endl;

  return 0;
}
