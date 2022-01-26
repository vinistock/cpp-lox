#ifndef SCANNER_H
#define SCANNER_H

#include "vm/token.h"
#include "vm/vm.h"
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

  bool still_going();
  void scan_token();
  char advance();
  char peek();
  void string();
  bool match(char expected);
  void add_token(TokenType type);
  void add_token(TokenType type, Object *literal);
};

#endif
