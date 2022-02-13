#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "literals/object.h"
#include "vm/token.h"

class Binary;
class Grouping;
class Literal;
class Unary;

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

class Binary : public Expr {
public:
  Binary(Expr *left, Token op, Expr *right)
      : left(left), op(op), right(right) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitBinaryExpr(*this);
  }

  Expr *left;
  const Token op;
  Expr *right;
};

class Grouping : public Expr {
public:
  Grouping(Expr *expression) : expression(expression) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitGroupingExpr(*this);
  }

  Expr *expression;
};

class Literal : public Expr {
public:
  Literal(std::shared_ptr<Object> value) : value(value) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitLiteralExpr(*this);
  }

  const std::shared_ptr<Object> value;
};

class Unary : public Expr {
public:
  Unary(Token op, Expr *right) : op(op), right(right) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitUnaryExpr(*this);
  }

  Token op;
  Expr *right;
};

#endif
