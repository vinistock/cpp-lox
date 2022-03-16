#ifndef PARSER_H
#define PARSER_H

#include "literals/boolean.h"
#include "vm/expr.h"
#include "vm/stmt.h"
#include "vm/token.h"
#include "vm/vm.h"
#include <exception>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Parser final {
public:
  Parser(std::vector<Token> tokens) : tokens(tokens) {}
  vector<shared_ptr<Stmt>> parse();

private:
  const std::vector<Token> tokens;
  int current = 0;

  shared_ptr<Expr> expression();
  shared_ptr<Expr> equality();
  shared_ptr<Expr> comparison();
  shared_ptr<Expr> term();
  shared_ptr<Expr> factor();
  shared_ptr<Expr> unary();
  shared_ptr<Expr> primary();
  bool match(std::vector<TokenType> types);
  bool check(TokenType type);
  Token advance();
  Token consume(TokenType type, string message);
  bool is_at_end();
  Token peek();
  Token previous();
  void synchronize();
  shared_ptr<Stmt> statement();
  shared_ptr<Stmt> print_statement();
  shared_ptr<Stmt> expression_statement();
};

class ParseError : public exception {
public:
  ParseError(Token token, const string &message)
      : token(token), message(message) {}

private:
  Token token;
  string message;
};

#endif
