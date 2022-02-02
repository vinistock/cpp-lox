#ifndef SCANNER_H
#define SCANNER_H

#include "literals/number.h"
#include "literals/string.h"
#include "vm/token.h"
#include "vm/vm.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Scanner final {
public:
  Scanner(std::string source) : source(source) {}
  std::vector<Token> scan_tokens();

private:
  const std::string source;
  std::vector<Token> tokens;

  int start = 0;
  int current = 0;
  int line = 1;

  std::map<std::string, TokenType> keywords = {
      {"and", TokenType::AND},       {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
      {"for", TokenType::FOR},       {"fun", TokenType::FUN},
      {"if", TokenType::IF},         {"nil", TokenType::NIL},
      {"or", TokenType::OR},         {"print", TokenType::PRINT},
      {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
      {"this", TokenType::THIS},     {"true", TokenType::TRUE},
      {"var", TokenType::VAR},       {"while", TokenType::WHILE},
  };

  bool still_going();
  void scan_token();
  char advance();
  char peek();
  char peek_next();
  void string();
  void number();
  void multi_line_comment();
  void identifier();
  bool is_digit(char c);
  bool is_alpha(char c);
  bool is_alpha_numeric(char c);
  bool match(char expected);
  void add_token(TokenType type);
  void add_token(TokenType type, std::shared_ptr<Object> literal);
};

#endif
