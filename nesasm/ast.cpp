#include "ast.h"
#include "utils.h"

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

  commend_begin = s.find("//");
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
      e << "Invalid operand: " << addr_mode_str; 
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

Instruction parse_instruction(std::vector<std::string> line) {
  // Parse operation
  Operation operation = OperationUtil::from_string(line[0]);
  if (operation == Operation::NotSupported) {
    std::ostringstream msg;
    msg << "Invalid instruction: " << line[0]; 
    throw std::runtime_error(msg.str());
  }

  // Parse addressing mode
  AddressMode addr_mode;
  std::string operand_str;
  if (line.size() > 1) {
    std::tie(addr_mode, operand_str) = parse_address_mode(line[1]);
  } else {
    addr_mode = AddressMode::Implied;
  }

  // Parse operand
  std::variant<uint16_t, std::string> operand = Utils::parse_operand(operand_str);

  return Instruction{addr_mode, operation, operand};
}

AST::AST(std::stringstream& asm_src) {
  // std::string curr_block = "global";
  std::string line;
  int line_num = 0;
  size_t instr_num = 0;

  while (std::getline(asm_src, line)) {
    line_num += 1;
    if (line.empty()) { continue; }

    // Split by space and remove comments
    std::vector<std::string> split_line = tokenize_line(line);
    if (split_line.empty()) { continue; }

    // Resolve each line type
    if (split_line[0].front() == '.') {  // Directive
      std::string directive = split_line[0].substr(1);
      std::vector<std::string> arguments;
      if (split_line[0].size() > 1) {
        arguments = std::vector<std::string>(split_line.begin() + 1, split_line.end());
      }

      if (directive == "reserve") {
        this->unresolevd_directives.push_back(Directive{ DirectiveType::Reserve, arguments });
        
      } else {
        std::string msg = "Invalid directive: " + directive + " at line " + std::to_string(line_num);
        throw std::runtime_error(msg);
      }

    } else if (split_line[0].back() == ':') {  // Label
      split_line[0].pop_back();
      this->label_map.insert({split_line[0], instr_num});

    } else {  // Instruction
      Instruction instr;
      try {
        instr = parse_instruction(split_line);
        instr.line = line_num;
      } catch (std::exception &e) {
        std::string msg = e.what();

        msg += " at line " + std::to_string(line_num);
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
    Instruction& instr = this->instr_list[i];

    std::cout << i;
    if (instr.location.has_value()) {
      std::cout << " (0x" << std::uppercase << std::hex << instr.location.value() << std::dec << ")";
      std::cout.unsetf(std::ios_base::uppercase);
    }

    std::cout
      << ": " << OperationUtil::to_string(instr.operation)
      << " " << AddressModeUtil::to_string(instr.address_mode);

      if (const std::string* op = std::get_if<std::string>(&instr.operand)) {
        std::cout << ' ' << *op;
      } else if (const std::uint16_t* op = std::get_if<uint16_t>(&instr.operand)) {
        std::cout << " " << std::uppercase << std::hex << static_cast<int>(*op) << std::dec;
        std::cout.unsetf(std::ios_base::uppercase);
      }

      std::cout << '\n';
  }

  std::cout << "\nLabels:\n";
  for (auto [k, v] : this->label_map) {
    std::cout << k << ": " << v << '\n';
  }
}
