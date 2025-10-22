#include "utils.h"

#include <stdexcept>
#include <regex>

std::variant<uint16_t, std::string> Utils::parse_operand(std::string operand_str) {
  std::variant<uint16_t, std::string> operand;

  int base;

  if (operand_str.empty()) {
    return "";
  } else if (operand_str.front() == '$') {
    base = 16;
    operand_str = operand_str.substr(1);
  } else if (operand_str.length() > 1 && (operand_str.substr(0, 2) == "0x" || operand_str.substr(0, 2) == "0X")) {
    base = 16;
    operand_str = operand_str.substr(2);
  } else if (operand_str.front() == '%') {
    base = 2;
    operand_str = operand_str.substr(1);
  } else if (operand_str.length() > 1 && (operand_str.substr(0, 2) == "0b" || operand_str.substr(0, 2) == "0B")) {
    base = 2;
    operand_str = operand_str.substr(2);
  } else if (operand_str.length() > 1 && operand_str.front() == '0') {
    base = 8;
    operand_str = operand_str.substr(1);
  } else if(std::isdigit(operand_str.front())) {
    base = 10;
  } else {
    static const std::regex symbol_regex(R"(^[A-Za-z_][A-Za-z0-9_]*(\.[A-Za-z0-9_]+)?$)");
    if (!std::regex_match(operand_str, symbol_regex)) {
        std::ostringstream msg;
        msg << "Invalid symbol operand: " << operand_str; 
        throw std::runtime_error(msg.str());
    }
    return operand_str;
  }

  try {
    size_t pos;
    operand = static_cast<uint16_t>(std::stoul(operand_str, &pos, base));

    if (pos != operand_str.length()) {
      std::ostringstream msg;
      msg << "Invalid numerical operand: " << operand_str << " base " << base; 
      throw std::runtime_error(msg.str());
    }
  } catch (...) {
    std::ostringstream msg;
    msg << "Invalid numerical operand: " << operand_str << " base " << base; 
    throw std::runtime_error(msg.str());
  }

  return operand;
}
