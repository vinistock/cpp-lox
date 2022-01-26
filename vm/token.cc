#include "vm/token.h"

std::string Token::to_string() const {
  std::string text = token_name() + " " + lexeme;

  if (literal) {
    text += " " + literal->to_string();
  }

  return text;
}

std::string Token::token_name() const {
  return token_names[static_cast<int>(type)];
}
