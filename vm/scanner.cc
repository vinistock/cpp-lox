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
    if (is_digit(c)) {
      number();
    } else {
      std::string message = "Unexpected character: ";
      message.push_back(c);
      Vm::error(line, message);
    }

    break;
  }

  return;
}

bool Scanner::is_digit(char c) { return c >= '0' && c <= '9'; }

void Scanner::number() {
  while (is_digit(peek())) {
    advance();
  }

  if (peek() == '.' && is_digit(peek_next())) {
    advance();

    while (is_digit(peek())) {
      advance();
    }
  }

  std::shared_ptr<Number> value = std::make_shared<Number>(
      std::stod(source.substr(start, current - start)));
  add_token(TokenType::NUMBER, value);
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

  std::shared_ptr<String> text = std::make_shared<String>(
      String(source.substr(start + 1, current - start - 2)));
  add_token(TokenType::STRING, text);
  return;
}

char Scanner::peek() {
  if (still_going()) {
    return source[current];
  }
  return '\0';
}

char Scanner::peek_next() {
  if (current + 1 >= source.length())
    return '\0';

  return source[current + 1];
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

void Scanner::add_token(TokenType type, std::shared_ptr<Object> literal) {
  std::string text = source.substr(start, current - start);
  tokens.push_back(Token(type, text, literal, line));
  return;
}
