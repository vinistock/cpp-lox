#include "vm/parser.h"

shared_ptr<Expr> Parser::expression() { return equality(); }

shared_ptr<Expr> Parser::equality() {
  shared_ptr<Expr> expr = comparison();

  std::vector<TokenType> types = {TokenType::BANG_EQUAL,
                                  TokenType::EQUAL_EQUAL};

  while (match(types)) {
    Token op = previous();
    shared_ptr<Expr> right = comparison();
    expr = make_shared<Binary>(Binary(expr, op, right));
  }

  return expr;
}

shared_ptr<Expr> Parser::comparison() {
  shared_ptr<Expr> expr = term();

  std::vector<TokenType> types = {TokenType::GREATER, TokenType::GREATER_EQUAL,
                                  TokenType::LESS, TokenType::LESS_EQUAL};

  while (match(types)) {
    Token op = previous();
    shared_ptr<Expr> right = term();
    expr = make_shared<Binary>(Binary(expr, op, right));
  }

  return expr;
}

shared_ptr<Expr> Parser::term() {
  shared_ptr<Expr> expr = factor();

  std::vector<TokenType> types = {TokenType::MINUS, TokenType::PLUS};

  while (match(types)) {
    Token op = previous();
    shared_ptr<Expr> right = factor();
    expr = make_shared<Binary>(Binary(expr, op, right));
  }

  return expr;
}

shared_ptr<Expr> Parser::factor() {
  shared_ptr<Expr> expr = unary();

  std::vector<TokenType> types = {TokenType::SLASH, TokenType::STAR};

  while (match(types)) {
    Token op = previous();
    shared_ptr<Expr> right = unary();
    expr = make_shared<Binary>(Binary(expr, op, right));
  }

  return expr;
}

shared_ptr<Expr> Parser::unary() {
  std::vector<TokenType> types = {TokenType::BANG, TokenType::MINUS};

  if (match(types)) {
    Token op = previous();
    shared_ptr<Expr> right = unary();
    return make_shared<Unary>(Unary(op, right));
  }

  return primary();
}

shared_ptr<Expr> Parser::primary() {
  std::vector<TokenType> types = {TokenType::FALSE};

  if (match(types))
    return make_shared<Literal>(Literal(make_shared<Boolean>(Boolean(false))));

  types = {TokenType::TRUE};
  if (match(types))
    return make_shared<Literal>(make_shared<Boolean>(Boolean(true)));

  types = {TokenType::NIL};
  if (match(types))
    return make_shared<Literal>(Literal(nullptr));

  types = {TokenType::NUMBER, TokenType::STRING};
  if (match(types)) {
    return make_shared<Literal>(Literal(previous().literal));
  }

  types = {TokenType::LEFT_PAREN};
  if (match(types)) {
    shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return make_shared<Grouping>(Grouping(expr));
  }

  throw ParseError(peek(), "Expect expression.");
}

bool Parser::match(std::vector<TokenType> types) {
  for (TokenType type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

bool Parser::check(TokenType type) {
  if (is_at_end()) {
    return false;
  }

  return peek().type == type;
}

Token Parser::advance() {
  if (!is_at_end()) {
    current++;
  }

  return previous();
}

Token Parser::consume(TokenType type, string message) {
  if (check(type))
    return advance();

  Token token = peek();
  Vm::error(token, message);
  throw ParseError(token, message);
}

void Parser::synchronize() {
  advance();

  while (!is_at_end()) {
    if (previous().type == TokenType::SEMICOLON)
      return;

    switch (peek().type) {
    case TokenType::CLASS:
    case TokenType::FUN:
    case TokenType::VAR:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::PRINT:
    case TokenType::RETURN:
      return;
    default:
      break;
    }

    advance();
  }
}

shared_ptr<Expr> Parser::parse() {
  try {
    return expression();
  } catch (ParseError &error) {
    return nullptr;
  }
}

bool Parser::is_at_end() { return peek().type == TokenType::ENDOF; }

Token Parser::peek() { return tokens[current]; }

Token Parser::previous() { return tokens[current - 1]; }