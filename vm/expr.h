#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "literals/object.h"
#include "literals/string.h"
#include "vm/token.h"

class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class Assign;
class Logical;

using namespace std;

template <class T> class Visitor {
public:
  virtual T visitBinaryExpr(Binary expr) = 0;
  virtual T visitGroupingExpr(Grouping expr) = 0;
  virtual T visitLiteralExpr(Literal expr) = 0;
  virtual T visitUnaryExpr(Unary expr) = 0;
  virtual T visitVariableExpr(Variable expr) = 0;
  virtual T visitAssignExpr(Assign expr) = 0;
  virtual T visitLogicalExpr(Logical expr) = 0;
};

class Expr {
public:
  virtual String accept(Visitor<String> *visitor) = 0;
  virtual shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) = 0;
};

class Binary final : public Expr {
public:
  Binary(shared_ptr<Expr> left, Token op, shared_ptr<Expr> right)
      : left(left), op(op), right(right) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitBinaryExpr(*this);
  }

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
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

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
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

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
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

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
    return visitor->visitUnaryExpr(*this);
  }

  Token op;
  shared_ptr<Expr> right;
};

class Variable final : public Expr {
public:
  Variable(Token name) : name(name) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitVariableExpr(*this);
  }

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
    return visitor->visitVariableExpr(*this);
  }

  Token name;
};

class Assign final : public Expr {
public:
  Assign(Token name, shared_ptr<Expr> value) : name(name), value(value) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitAssignExpr(*this);
  }

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
    return visitor->visitAssignExpr(*this);
  }

  Token name;
  shared_ptr<Expr> value;
};

class Logical final : public Expr {
public:
  Logical(shared_ptr<Expr> left, Token op, shared_ptr<Expr> right)
      : left(left), op(op), right(right) {}

  String accept(Visitor<String> *visitor) {
    return visitor->visitLogicalExpr(*this);
  }

  shared_ptr<Object> accept(Visitor<shared_ptr<Object>> *visitor) {
    return visitor->visitLogicalExpr(*this);
  }

  shared_ptr<Expr> left;
  const Token op;
  shared_ptr<Expr> right;
};

#endif
