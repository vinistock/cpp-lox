#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "literals/boolean.h"
#include "literals/number.h"
#include "literals/object.h"
#include "vm/environment.h"
#include "vm/errors.h"
#include "vm/expr.h"
#include "vm/stmt.h"
#include "vm/vm.h"
#include <string>
#include <typeinfo>

using namespace std;

class Interpreter : public Visitor<shared_ptr<Object>>,
                    public StmtVisitor<void> {
public:
  shared_ptr<Object> visitLiteralExpr(Literal expr);
  shared_ptr<Object> visitGroupingExpr(Grouping expr);
  shared_ptr<Object> visitUnaryExpr(Unary expr);
  shared_ptr<Object> visitBinaryExpr(Binary expr);

  void visitVarStmt(Var stmt);
  shared_ptr<Object> visitVariableExpr(Variable expr);
  shared_ptr<Object> visitAssignExpr(Assign expr);
  void visitBlockStmt(Block stmt);
  void visitIfStmt(If stmt);

  void interpret(vector<shared_ptr<Stmt>> statements);
  void execute_block(vector<shared_ptr<Stmt>> statements,
                     shared_ptr<Environment> environment);

private:
  shared_ptr<Environment> environment = make_shared<Environment>(Environment());

  shared_ptr<Object> evaluate(shared_ptr<Expr> expr);
  bool is_truthy(shared_ptr<Object> obj);
  bool is_equal(shared_ptr<Object> a, shared_ptr<Object> b);
  void check_number_operand(Token op, shared_ptr<Object> operand);
  void check_number_operands(Token op, shared_ptr<Object> left,
                             shared_ptr<Object> right);
  string stringify(shared_ptr<Object> object);
  void visitExpressionStmt(Expression stmt);
  void visitPrintStmt(Print stmt);
  void execute(shared_ptr<Stmt> stmt);
};

#endif
