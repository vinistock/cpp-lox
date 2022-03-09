#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "literals/boolean.h"
#include "literals/number.h"
#include "literals/object.h"
#include "vm/expr.h"
#include "vm/vm.h"
#include <string>
#include <typeinfo>

using namespace std;

class Interpreter : public Visitor<shared_ptr<Object>> {
public:
  shared_ptr<Object> visitLiteralExpr(Literal expr);
  shared_ptr<Object> visitGroupingExpr(Grouping expr);
  shared_ptr<Object> visitUnaryExpr(Unary expr);
  shared_ptr<Object> visitBinaryExpr(Binary expr);
  void interpret(shared_ptr<Expr> expression);

private:
  shared_ptr<Object> evaluate(shared_ptr<Expr> expr);
  bool is_truthy(shared_ptr<Object> obj);
  bool is_equal(shared_ptr<Object> a, shared_ptr<Object> b);
  void check_number_operand(Token op, shared_ptr<Object> operand);
  void check_number_operands(Token op, shared_ptr<Object> left,
                             shared_ptr<Object> right);
  string stringify(shared_ptr<Object> object);
};

class RuntimeError : public exception {
public:
  RuntimeError(Token op, const string &message) : op(op), message(message) {}

  Token op;
  string message;
};

#endif
