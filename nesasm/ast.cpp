#include "ast.h"

#include <stdexcept>
#include <regex>
#include <utility>
#include <iostream>
#include <iomanip>

// Splits the line by whitespaces and removes any comments
std::vector<std::string> tokenize_line(std::string s) {
  std::vector<std::string> split;

  auto commend_begin = s.find(';');
  if (commend_begin != std::string::npos) {
    s = s.substr(0, commend_begin);
  }

  auto it = s.begin();
  while (it != s.end()) {
    auto word_begin = std::find_if(it, s.end(), [](unsigned char ch) {
      return !std::isspace(ch);
    });

    auto word_end = std::find_if(word_begin, s.end(), [](unsigned char ch) {
      return std::isspace(ch);
    });

    if (word_begin != word_end) {
      split.emplace_back(word_begin, word_end);
    }

    it = word_end;
  }

  return split;
}

std::pair<AddressMode, std::string> parse_address_mode(std::string addr_mode_str) {
  AddressMode addr_mode;
  std::string operand_str;

  if (addr_mode_str == "A") {  // Accumulator
    addr_mode = AddressMode::Accumulator;

  } else if (addr_mode_str.front() == '#') {  // Immediate
    addr_mode = AddressMode::Immediate;
    operand_str = addr_mode_str.substr(1);

  } else if (addr_mode_str.front() == '(') {  // Indirect variant
    if (addr_mode_str.length() >= 4 && addr_mode_str.compare(addr_mode_str.length() - 3, 3, ",X)") == 0) {  // Indirect X
      addr_mode = AddressMode::IndirectX;
      operand_str = addr_mode_str.substr(1, addr_mode_str.length() - 3);

    } else if (addr_mode_str.size() >= 4 && addr_mode_str.compare(addr_mode_str.length() - 3, 3, "),Y") == 0) {  // Indirect Y
      addr_mode = AddressMode::IndirectY;
      operand_str = addr_mode_str.substr(1, addr_mode_str.length() - 3);

    } else if (addr_mode_str.back() == ')') {  // Indirect
      addr_mode = AddressMode::Indirect;
      operand_str = addr_mode_str.substr(1, addr_mode_str.length() - 1);

    } else {  // No match
      std::ostringstream e;
      e << "Invalid operand: " << addr_mode_str << '\n'; 
      throw std::runtime_error(e.str());
    }

  } else if (addr_mode_str.length() >= 2 && addr_mode_str.compare(addr_mode_str.length() - 2, 2, ",X") == 0) {  // AbsoluteX or zeropageX
    addr_mode = AddressMode::AmbiguousZeropageXOrAbsoluteX;
    operand_str = addr_mode_str.substr(0, addr_mode_str.length() - 2);

  } else if (addr_mode_str.length() >= 2 && addr_mode_str.compare(addr_mode_str.length() - 2, 2, ",Y") == 0) {  // AbsoluteY or zeropageY
    addr_mode = AddressMode::AmbiguousZeropageYOrAbsoluteY;
    operand_str = addr_mode_str.substr(0, addr_mode_str.length() - 2);

  } else {  // Absolute, zeropage, or relative
    addr_mode = AddressMode::AmbiguousZeropageOrAbsoluteOrRelative;
    operand_str = addr_mode_str;
  }

  return std::pair(addr_mode, operand_str);
}

std::variant<uint16_t, std::string> parse_operand(std::string operand_str) {
  std::variant<uint16_t, std::string> operand;

  if (operand_str.empty()) {
    operand = "";
      // std::ostringstream e;
      // e << "Invalid empty operand\n"; 
      // throw std::runtime_error(e.str());

  } else if (std::isdigit(operand_str.front())) {
    try {
      size_t pos;
      operand = static_cast<uint16_t>(std::stoul(operand_str, &pos));

      if (pos != operand_str.length()) {
        std::ostringstream e;
        e << "Invalid decimal numerical operand: " << operand_str << '\n'; 
        throw std::runtime_error(e.str());
      }
    } catch (...) {
      std::ostringstream e;
      e << "Invalid decimal numerical operand: " << operand_str << '\n'; 
      throw std::runtime_error(e.str());
    }

  } else if (operand_str.front() == '$') {
    try {
      size_t pos;
      operand = static_cast<uint16_t>(std::stoul(operand_str.substr(1), &pos, 16));

      if (pos != operand_str.length() - 1) {
        std::ostringstream e;
        e << "Invalid hexadecimal numerical operand: " << operand_str << '\n'; 
        throw std::runtime_error(e.str());
      }
    } catch (...) {
      std::ostringstream e;
      e << "Invalid hexadecimal numerical operand: " << operand_str << '\n'; 
      throw std::runtime_error(e.str());
    }

  } else if (operand_str.front() == '@') {
    try {
      size_t pos;
      operand = static_cast<uint16_t>(std::stoul(operand_str.substr(1), &pos, 8));

      if (pos != operand_str.length() - 1) {
        std::ostringstream e;
        e << "Invalid octal numerical operand: " << operand_str << '\n'; 
        throw std::runtime_error(e.str());
      }
    } catch (...) {
      std::ostringstream e;
      e << "Invalid octal numerical operand: " << operand_str << '\n'; 
      throw std::runtime_error(e.str());
    }

  } else if (operand_str.front() == '$') {
    try {
      size_t pos;
      operand = static_cast<uint16_t>(std::stoul(operand_str.substr(1), &pos, 2));

      if (pos != operand_str.length() - 1) {
        std::ostringstream e;
        e << "Invalid binary numerical operand: " << operand_str << '\n'; 
        throw std::runtime_error(e.str());
      }
    } catch (...) {
      std::ostringstream e;
      e << "Invalid binary numerical operand: " << operand_str << '\n'; 
      throw std::runtime_error(e.str());
    }

  } else {
    static const std::regex symbol_regex(R"(^[A-Za-z_][A-Za-z0-9_]*$)");
    if (!std::regex_match(operand_str, symbol_regex)) {
        std::ostringstream e;
        e << "Invalid symbol operand: " << operand_str << '\n'; 
        throw std::runtime_error(e.str());
    }
    operand = operand_str;
  }

  return operand;
}

Instruction parse_instruction(std::vector<std::string> line) {
  // Parse operation
  Operation operation = OperationUtil::from_string(line[0]);
  if (operation == Operation::NotSupported) {
    std::ostringstream msg;
    msg << "Invalid instruction: " << line[0] << '\n'; 
    throw std::runtime_error(msg.str());
  }
  // try {
  //   operation = operation_parse.at(line[0]);
  // } catch (std::out_of_range e) {
  //   std::ostringstream msg;
  //   msg << "Invalid instruction: " << line[0] << '\n'; 
  //   throw std::runtime_error(msg.str());
  // }

  // Parse addressing mode
  AddressMode addr_mode;
  std::string operand_str;
  if (line.size() > 1) {
    std::tie(addr_mode, operand_str) = parse_address_mode(line[1]);
  } else {
    addr_mode = AddressMode::Implied;
  }

  // Parse operand
  std::variant<uint16_t, std::string> operand = parse_operand(operand_str);

  return Instruction{addr_mode, operation, operand};
}

AST::AST(std::stringstream& asm_src) {
  std::string line;
  int line_num = 0;
  int instr_num = 0;

  while (std::getline(asm_src, line)) {
    line_num += 1;
    if (line.empty()) { continue; }

    // Split by space and remove comments
    std::vector<std::string> split_line = tokenize_line(line);

    // Resolve each line type
    if (split_line[0].front() == '.') {  // Directive
      // Currently no directives implemented

    } else if (split_line[0].back() == ':') {  // Label
      split_line[0].pop_back();
      this->symbol_map.insert({split_line[0], instr_num});

    } else {  // Instruction
      Instruction instr;
      try {
        instr = parse_instruction(split_line);
      } catch (std::exception &e) {
        std::string msg = e.what();
        
        if (!msg.empty() && msg.back() == '\n') {
          msg.pop_back();
        }

        msg += " at line " + std::to_string(line_num) + '\n';
        throw std::runtime_error(msg);
      }

      if (const std::string* op = std::get_if<std::string>(&instr.operand)) {
        this->unresolved_symbols.insert(*op);
      }

      this->instr_list.push_back(instr);
      instr_num += 1;
    }
  }
}

void AST::print() {
  std::cout << "Instructions:\n";
  for (int i = 0; i < this->instr_list.size(); i++) {
    std::cout
      << i << ":"
      << " " << OperationUtil::to_string(this->instr_list[i].operation)
      << " " << AddressModeUtil::to_string(this->instr_list[i].address_mode);

      if (const std::string* op = std::get_if<std::string>(&this->instr_list[i].operand)) {
        std::cout << ' ' << *op;
      } else if (const std::uint16_t* op = std::get_if<uint16_t>(&this->instr_list[i].operand)) {
        std::cout << " " << std::uppercase << std::hex << static_cast<int>(*op) << std::dec;
        std::cout.unsetf(std::ios_base::uppercase);
      }

      std::cout << '\n';
  }

  std::cout << "\nLabels:\n";
  for (auto [k, v] : this->symbol_map) {
    std::cout << k << ": " << v << '\n';
  }
}
