#include "assembler.h"

Assembler::Assembler() {}

void Assembler::build_ast(std::stringstream& asm_src) {
  this->ast = AST(asm_src);
}

void Assembler::resole_symbols() {
  for (auto sym : this->ast.unresolved_symbols) {
    if (this->ast.label_map.count(sym)) {
      // Symbol is an already resolved label
      continue;
    }

    std::optional<uint16_t> loc = this->allocator.reserve_block(1);
    if (!loc.has_value()) {
      std::ostringstream msg;
      msg << "No available memory for symbol: " << sym << '\n'; 
      throw std::runtime_error(msg.str());
    }

    this->ast.variable_map.insert({sym, loc.value()});
  }

  this->ast.unresolved_symbols.clear();
}
