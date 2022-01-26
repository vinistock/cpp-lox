#include "vm/token.h"

std::string Token::to_string() const {
  return token_name() + " " + lexeme + " " + literal->to_string();
}

std::string Token::token_name() const {
  return token_names[static_cast<int>(type)];
}
