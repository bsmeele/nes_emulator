#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <variant>
#include <string>

namespace Utils{
  std::variant<uint16_t, std::string> parse_operand(std::string operand_str);
}

#endif  // UTILS_H
