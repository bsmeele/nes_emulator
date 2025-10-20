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

void Assembler::allocate_variables() {
  for (auto sym : this->ast.unresolved_symbols) {

    if (this->ast.label_map.count(sym)) {
      // Symbol is an existing label
      continue;
    }

    if (this->ast.variable_map.count(sym)) {
      // Symbol is already allocated
      continue;
    }
    std::cout << sym << "\n";

    std::optional<uint16_t> loc = this->allocator.reserve_block(1);
    if (!loc.has_value()) {
      std::ostringstream msg;
      msg << "No available memory for symbol: " << sym; 
      throw std::runtime_error(msg.str());
    }

    this->ast.variable_map.insert({sym, loc.value()});
  }

  this->ast.unresolved_symbols.clear();
}

void Assembler::resolve_variable_operands() {
  for (auto& instr : this->ast.instr_list) {
    if (!instr.operand.has_value() || std::holds_alternative<uint16_t>(instr.operand.value())) {
      continue;  // Operand is a number or no operand is present
    }

    std::string s = std::get<std::string>(*instr.operand);
    if (this->ast.variable_map.count(s) == 0) { continue; }  // The operand is not a variable

    instr.operand = this->ast.variable_map[s];
  }
}

void Assembler::resolve_ambiguity() {
  for (auto& instr : this->ast.instr_list) {
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

void Assembler::allocate_codeblocks() {
  for (int i = 0; i < this->ast.instr_list.size(); i++) {
    Instruction& cur = this->ast.instr_list[i];

    if (i == 0) {
      cur.location = 0x8000;
    } else {
      Instruction& prev = this->ast.instr_list[i-1];

      std::optional<size_t> size = AddressModeUtil::get_size(prev.address_mode);
      if (!size.has_value()) {
        std::string msg = "Could not get size for instruction at line " + std::to_string(prev.line);
        throw std::runtime_error(msg);
      }

      cur.location = prev.location.value() + size.value() + 1;
    }
  }
}

void Assembler::resolve_labels() {
  for (int i = 0; i < this->ast.instr_list.size(); i++) {
    auto& instr = this->ast.instr_list[i];

    if (!instr.operand.has_value() || std::holds_alternative<uint16_t>(instr.operand.value())) {
      continue;  // Operand is a number or is empty
    }

    std::string s = std::get<std::string>(*instr.operand);
    if (this->ast.label_map.count(s) == 0) { continue; }  // The operand is not a label
    
    // The label map points to the next instruction after that label
    // The instruction knows where it is located in memory, thus what the jump location should be
    std::optional<uint16_t> addr = this->ast.instr_list[this->ast.label_map[s]].location;
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

std::vector<uint8_t> Assembler::generate_machine_code() {
  std::vector<uint8_t> machine_code;
  for (int i = 0; i < this->ast.instr_list.size(); i++) {
    auto& instr = this->ast.instr_list[i];

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

  return machine_code;
}
