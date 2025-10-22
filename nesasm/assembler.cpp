#include "assembler.h"
#include "directive.h"
#include "utils.h"
#include "shared/opcode_lookup.h"

#include <iostream>

Assembler::Assembler() {}

void Assembler::build_ast(std::stringstream& asm_src) {
  this->ast = AST(asm_src);
}

void Assembler::resolve_directives() {
  for (auto dir : this->ast.unresolevd_directives) {
    switch (dir.directive) {
      case DirectiveType::Reserve: {
        if (dir.arguments.empty()) {
          std::string msg = "No arguments given with .reserve on line " + std::to_string(dir.line);
          throw std::runtime_error(msg);
        }

        std::variant<uint16_t, std::string> start = Utils::parse_operand(dir.arguments[0]);
        std::variant<uint16_t, std::string> end = dir.arguments.size() > 1 ? Utils::parse_operand(dir.arguments[1]) : start;

        const uint16_t* s = std::get_if<uint16_t>(&start);
        const uint16_t* e = std::get_if<uint16_t>(&end);

        if (s && e) {
          this->allocator.reserve_at(*s, *e - *s + 1);

        } else {
          std::string msg = "Invalid operands: " + std::get<std::string>(start) + " " + std::get<std::string>(end) + " at line " + std::to_string(dir.line);
            + " for .reserve directive";
          throw std::runtime_error(msg);
        }

        break;
      }
      default:
        std::string msg = "Invalid directive: " + DirectiveUtil::to_string(dir.directive) + " at line " + std::to_string(dir.line);
        throw std::runtime_error(msg);
    }
  }

  this->ast.unresolevd_directives.clear();
}

void Assembler::resolve_label_ambiguity() {
  for (size_t b = 0; b < this->ast.block_list.size(); b++) {
    for (size_t i = 0; i < this->ast.block_list[b].instr_list.size(); i++) {
      CodeBlock& block = this->ast.block_list[b];
      Instruction& instr = this->ast.block_list[b].instr_list[i];

      if (!instr.operand.has_value() || std::holds_alternative<uint16_t>(instr.operand.value())) {
        continue;  // Operand is not a label
      }

      std::string sym = std::get<std::string>(*instr.operand);
      if (this->ast.label_map.count(sym) != 0) {
        continue;  // Label is not ambiguous

      } else if (this->ast.label_map.count(block.name + "." + sym)) {
        // Label refers to a local label
        instr.operand = block.name + "." + sym;
        
      } else if (this->ast.label_map.count("global." + sym)) {
        // Label refers to a global label
        instr.operand = "global." + sym;

      } else if (this->ast.block_map.count(sym)) {
        // Label refers to code block
        size_t block_num = this->ast.block_map.at(sym);
        instr.operand = sym + "." + this->ast.block_list[block_num].entry;
      }

      // Else: symbol is not a label
    }
  }
}

void Assembler::allocate_variables() {
  for (size_t b = 0; b < this->ast.block_list.size(); b++) {
    for (size_t i = 0; i < this->ast.block_list[b].instr_list.size(); i++) {
      CodeBlock& block = this->ast.block_list[b];
      Instruction& instr = this->ast.block_list[b].instr_list[i];
      
      if (!instr.operand.has_value() || std::holds_alternative<uint16_t>(instr.operand.value())) {
        continue;  // Operand is a number or no operand is present
      }

      std::string sym = std::get<std::string>(*instr.operand);

      if (this->ast.label_map.count(sym) != 0) {
        continue;  // Symbol is a label
      }

      if (this->ast.variable_map.count(block.name + "." + sym) != 0) {
        // Symbol is a local variable that is already allocated
        instr.operand = block.name + "." + sym;
        continue;
      } else if (this->ast.variable_map.count("global." + sym) != 0) {
        // Symbol is a global variable that is already allocated
        instr.operand = "global." + sym;
        continue;
      }

      std::optional<uint16_t> loc = this->allocator.reserve_block(1);
      if (!loc.has_value()) {
        std::ostringstream msg;
        msg << "No available memory for symbol: " << sym; 
        throw std::runtime_error(msg.str());
      }

      this->ast.variable_map.insert({block.name + "." + sym, loc.value()});
      instr.operand = block.name + "." + sym;
    }
  }
}

void Assembler::resolve_variable_operands() {
  for (auto& block : this->ast.block_list) {
    for (auto& instr : block.instr_list) {
      if (!instr.operand.has_value() || std::holds_alternative<uint16_t>(instr.operand.value())) {
        continue;  // Operand is a number or no operand is present
      }

      std::string s = std::get<std::string>(*instr.operand);
      if (this->ast.variable_map.count(s) == 0) { continue; }  // The operand is not a variable

      instr.operand = this->ast.variable_map[s];
    }
  }
}

void Assembler::resolve_address_mode_ambiguity() {
  for (auto& block : this->ast.block_list) {
    for (auto& instr : block.instr_list) {
      switch (instr.address_mode) {
        case AddressMode::AmbiguousZeropageOrAbsoluteOrRelative:
          if (OperationUtil::is_branch(instr.operation)) {
            instr.address_mode = AddressMode::Relative;

          } else if (OperationUtil::is_jump(instr.operation)) {
            instr.address_mode = AddressMode::Absolute;

          } else if (instr.operand.has_value() && std::holds_alternative<uint16_t>(instr.operand.value())) {
            uint16_t op = std::get<uint16_t>(*instr.operand);
            if (op > 0xFF) {
              instr.address_mode = AddressMode::Absolute;
            } else {
              instr.address_mode = AddressMode::ZeroPage;
            }

          } else {
            std::string msg = "Could not resolve " + AddressModeUtil::to_string(instr.address_mode) + " at line " + std::to_string(instr.line);
            throw std::runtime_error(msg);
          }
          break;

        case AddressMode::AmbiguousZeropageXOrAbsoluteX:
          if (instr.operand.has_value() && std::holds_alternative<uint16_t>(instr.operand.value())) {
            uint16_t op = std::get<uint16_t>(*instr.operand);
            if (op > 0xFF) {
              instr.address_mode = AddressMode::AbsoluteX;
            } else {
              instr.address_mode = AddressMode::ZeroPageX;
            }
            } else {
              std::string msg = "Could not resolve " + AddressModeUtil::to_string(instr.address_mode) + " at line " + std::to_string(instr.line);
              throw std::runtime_error(msg);
            }
            break;

          case AddressMode::AmbiguousZeropageYOrAbsoluteY:
            if (instr.operand.has_value() && std::holds_alternative<uint16_t>(instr.operand.value())) {
              uint16_t op = std::get<uint16_t>(*instr.operand);
              if (op > 0xFF) {
                instr.address_mode = AddressMode::AbsoluteY;
              } else {
                instr.address_mode = AddressMode::ZeroPageY;
              }
            }
            break;

        default:
          // Address mode is already resolevd
          break;
      }
    }
  }
}

void Assembler::allocate_codeblocks() {
  uint16_t addr = 0x8000;
  for (size_t b = 0; b < this->ast.block_list.size(); b++) {
  for (size_t i = 0; i < this->ast.block_list[b].instr_list.size(); i++) {
      Instruction& instr = this->ast.block_list[b].instr_list[i];

      instr.location = addr;

      std::optional<size_t> size = AddressModeUtil::get_size(instr.address_mode);
      if (!size.has_value()) {
          std::string msg = "Could not get size for instruction at line " + std::to_string(instr.line);
          throw std::runtime_error(msg);
      }

      addr += size.value() + 1;
    }
  }
}

void Assembler::resolve_labels() {
  for (size_t b = 0; b < this->ast.block_list.size(); b++) {
    for (size_t i = 0; i < this->ast.block_list[b].instr_list.size(); i++) {
      auto& instr = this->ast.block_list[b].instr_list[i];

      if (!instr.operand.has_value() || std::holds_alternative<uint16_t>(instr.operand.value())) {
        continue;  // Operand is a number or is empty
      }

      std::string s = std::get<std::string>(*instr.operand);
      if (this->ast.label_map.count(s) == 0) { continue; }  // The operand is not a label

      // The label map points to the next instruction after that label
      // The instruction knows where it is located in memory, thus what the jump location should be
      size_t block_num;
      size_t instr_num;
      std::tie(block_num, instr_num) = this->ast.label_map[s];
      std::optional<uint16_t> addr = this->ast.block_list[block_num].instr_list[instr_num].location;
      if(!addr.has_value()) {
        std::string msg = "Attempted to locate unplaced instruction";
        throw std::runtime_error(msg);
      }

      if (instr.address_mode == AddressMode::Relative) {
        // Relative addressing uses an offset as its argument
        // The offset can be calculated by subtracting the location of the next instruction from the jump location

        if (!instr.location.has_value()) {
          std::string msg = "Attempted to locate unplaced instruction";
          throw std::runtime_error(msg);
        }

        std::optional<size_t> size = AddressModeUtil::get_size(instr.address_mode);
        if (!size.has_value()) {
          throw std::runtime_error("Could not determine size of instruction");
        }

        int offset = addr.value() - (instr.location.value() + size.value() + 1);
        if (offset > 127 || offset < -128) {
          std::string msg = "Offset " + std::to_string(offset) + " too large for relative addressing";
          throw std::runtime_error(msg);
        }

        instr.operand = static_cast<uint8_t>(offset);

      } else if (instr.address_mode == AddressMode::Absolute) {
        instr.operand = addr.value();
      } else {
        // The jump instruction also allows for indirect addressing.
        // At this point, using a label alias with an indirect addressed jump instruction is not supported

        throw std::runtime_error("Label alias not supported for " + AddressModeUtil::to_string(instr.address_mode) + " addressing");
      }
    }
  }
}

std::vector<uint8_t> Assembler::generate_machine_code() {
  std::vector<uint8_t> machine_code;
  for (size_t b = 0; b < this->ast.block_list.size(); b++) {
    for (size_t i = 0; i < this->ast.block_list[b].instr_list.size(); i++) {
      auto& instr = this->ast.block_list[b].instr_list[i];

      // Opcode
      if (opcode_lookup.count(std::pair(instr.address_mode, instr.operation))) {
        machine_code.push_back(opcode_lookup.at(std::pair(instr.address_mode, instr.operation)));
      } else {
        std::string msg = "Could not get opcode for "
          + OperationUtil::to_string(instr.operation)
          + " " + AddressModeUtil::to_string(instr.address_mode)
          + " at line " + std::to_string(instr.line);
        throw std::runtime_error(msg);
      }

      if (!instr.operand.has_value()) { continue; }

      // Operand
      if (std::holds_alternative<uint16_t>(instr.operand.value())) {
        uint16_t op = std::get<uint16_t>(*instr.operand);
        std::optional<size_t> size = AddressModeUtil::get_size(instr.address_mode);
        if (!size.has_value()) {
          std::string msg = "Could not get size for instruction "
            + OperationUtil::to_string(instr.operation)
            + " " + AddressModeUtil::to_string(instr.address_mode)
            + " at line " + std::to_string(instr.line);
          throw std::runtime_error(msg);
        }

        while (size.value() > 0) {
          machine_code.push_back(static_cast<uint8_t>(op & 0xFF));
          op = op >> 8;
          size = size.value() - 1;
        }
      } else {
        std::string msg = "Could not generate machine code for operator "
          + std::get<std::string>(instr.operand.value())
          + " at line " + std::to_string(instr.line);
          throw std::runtime_error(msg);
      }
    }
  }
  return machine_code;
}

std::vector<uint8_t> Assembler::assemble(std::stringstream& source) {
  this->build_ast(source);
  this->resolve_directives();
  this->resolve_label_ambiguity();
  this->allocate_variables();
  this->resolve_variable_operands();
  this->resolve_address_mode_ambiguity();
  this->allocate_codeblocks();
  this->resolve_labels();

  std::vector<uint8_t> code = this->generate_machine_code();

  uint8_t prg_rom_size = 1;
  if (code.size() > 0x7FFC) {
    std::string msg = "Program of size " + std::to_string(code.size()) + " could not fit in NROM";
    throw std::runtime_error(msg);
  } else if (code.size() > 0x3FFC) {
    prg_rom_size = 2;
  }

  std::vector<uint8_t> bin(0x10 + prg_rom_size * 0x4000, 0x00);

  // Make header
  std::vector<uint8_t> header = { 0x4E, 0x45, 0x53, 0x1A, prg_rom_size, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
  std::copy(header.begin(), header.end(), bin.begin());

  // Write PRG ROM
  std::copy(code.begin(), code.end(), bin.begin() + header.size());

  // End with jump to start
  std::vector<uint8_t> jump_to_start = { 0x4C, 0x00, 0x80, 0x00 };
  std::copy(jump_to_start.begin(), jump_to_start.end(), bin.end() - jump_to_start.size());

  return bin;
}
