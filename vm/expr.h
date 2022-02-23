#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "literals/object.h"
#include "literals/string.h"
#include "vm/token.h"

class Binary;
class Grouping;
class Literal;
class Unary;

using namespace std;

template <class T> class Visitor {
public:
  virtual T visitBinaryExpr(Binary expr) = 0;
  virtual T visitGroupingExpr(Grouping expr) = 0;
  virtual T visitLiteralExpr(Literal expr) = 0;
  virtual T visitUnaryExpr(Unary expr) = 0;
};

class Expr {
public:
  virtual String accept(Visitor<String> *visitor) = 0;
};

class Binary final : public Expr {
public:
  Binary(shared_ptr<Expr> left, Token op, shared_ptr<Expr> right)
      : left(left), op(op), right(right) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitBinaryExpr(*this);
  }

  shared_ptr<Expr> left;
  const Token op;
  shared_ptr<Expr> right;
};

class Grouping final : public Expr {
public:
  Grouping(shared_ptr<Expr> expression) : expression(expression) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitGroupingExpr(*this);
  }

  shared_ptr<Expr> expression;
};

class Literal final : public Expr {
public:
  Literal(shared_ptr<Object> value) : value(value) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitLiteralExpr(*this);
  }

  const shared_ptr<Object> value;
};

class Unary final : public Expr {
public:
  Unary(Token op, shared_ptr<Expr> right) : op(op), right(right) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitUnaryExpr(*this);
  }

  Token op;
  shared_ptr<Expr> right;
};

#endif
