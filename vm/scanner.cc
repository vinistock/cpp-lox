#include "vm/scanner.h"
#include <__nullptr>

std::vector<Token> Scanner::scan_tokens() {
  while (still_going()) {
    start = current;
    scan_token();
  }

  tokens.push_back(Token(TokenType::ENDOF, "", nullptr, line));
  return tokens;
}

bool Scanner::still_going() { return current < source.length(); }

void Scanner::scan_token() {
  char c = advance();

  switch (c) {
  case '(':
    add_token(TokenType::LEFT_PAREN);
    break;
  case ')':
    add_token(TokenType::RIGHT_PAREN);
    break;
  case '{':
    add_token(TokenType::LEFT_BRACE);
    break;
  case '}':
    add_token(TokenType::RIGHT_BRACE);
    break;
  case ',':
    add_token(TokenType::COMMA);
    break;
  case '.':
    add_token(TokenType::DOT);
    break;
  case '-':
    add_token(TokenType::MINUS);
    break;
  case '+':
    add_token(TokenType::PLUS);
    break;
  case ';':
    add_token(TokenType::SEMICOLON);
    break;
  case '*':
    add_token(TokenType::STAR);
    break;
  case '!':
    add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  case '=':
    add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  case '<':
    add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  case '>':
    add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;
  case '/':
    // Parsing comments
    if (match('/')) {
      while (peek() != '\n' && still_going()) {
        advance();
      }
      // Parsing division
    } else {
      add_token(TokenType::SLASH);
    }
    break;
  case '"':
    string();
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    line++;
    break;
  default:
    std::string message = "Unexpected character: ";
    message.push_back(c);
    Vm::error(line, message);
    break;
  }

  return;
}

void Scanner::string() {
  while (peek() != '"' && still_going()) {
    if (peek() == '\n') {
      line++;
    }
    advance();
  }

  if (!still_going()) {
    Vm::error(line, "Unterminated string.");
    return;
  }

  advance();

  // std::string text = source.substr(start + 1, current - 1);
  // add_token(TokenType::STRING, text);
  return;
}

char Scanner::peek() {
  if (still_going()) {
    return source[current];
  }
  return '\0';
}

bool Scanner::match(char expected) {
  if (!still_going())
    return false;

  if (source[current] != expected)
    return false;

  current++;
  return true;
}

char Scanner::advance() { return source[current++]; }

void Scanner::add_token(TokenType type) {
  add_token(type, nullptr);
  return;
}

void Scanner::add_token(TokenType type, Object *literal) {
  std::string text = source.substr(start, current);
  tokens.push_back(Token(type, text, literal, line));
  return;
}
