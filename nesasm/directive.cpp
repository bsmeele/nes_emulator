#include "directive.h"

std::string DirectiveUtil::to_string(DirectiveType dir) {
  switch (dir) {
    case DirectiveType::Block:
      return "Block";
    case DirectiveType::Reserve:
      return "Reserve";
    case DirectiveType::Constant:
      return "Constant";
    case DirectiveType::Global:
      return "Global";
    default:
      return "Unrecognized";
  }
}
