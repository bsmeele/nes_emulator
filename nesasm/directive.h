#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include <vector>
#include <string>

enum class DirectiveType {
  Block,
  Reserve,
  Constant,
  Global,
};

struct Directive {
  DirectiveType directive;
  std::vector<std::string> arguments;
  size_t line;
};

namespace DirectiveUtil {
  std::string to_string(DirectiveType dir);
}

#endif  // DIRECTIVE_H
