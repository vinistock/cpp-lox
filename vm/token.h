#ifndef TOKEN_H
#define TOKEN_H

#include "vm/object.h"
#include <string>

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  ENDOF
};

class Token final {
public:
  Token(TokenType type, std::string lexeme, Object *literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}

  std::string to_string() const;

private:
  const TokenType type;
  const std::string lexeme;
  const Object *literal;
  const int line;
  const std::string token_names[39] = {
      "LEFT_PAREN", "RIGHT_PAREN",   "LEFT_BRACE", "RIGHT_BRACE", "COMMA",
      "DOT",        "MINUS",         "PLUS",       "SEMICOLON",   "SLASH",
      "STAR",       "BANG",          "BANG_EQUAL", "EQUAL",       "EQUAL_EQUAL",
      "GREATER",    "GREATER_EQUAL", "LESS",       "LESS_EQUAL",  "IDENTIFIER",
      "STRING",     "NUMBER",        "AND",        "CLASS",       "ELSE",
      "FALSE",      "FUN",           "FOR",        "IF",          "NIL",
      "OR",         "PRINT",         "RETURN",     "SUPER",       "THIS",
      "TRUE",       "VAR",           "WHILE",      "ENDOF"};

  std::string token_name() const;
};

#endif
