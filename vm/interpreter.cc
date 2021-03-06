#include "vm/interpreter.h"

shared_ptr<Object> Interpreter::visitLiteralExpr(Literal expr) {
  return expr.value;
}

shared_ptr<Object> Interpreter::visitGroupingExpr(Grouping expr) {
  return evaluate(expr.expression);
}

shared_ptr<Object> Interpreter::evaluate(shared_ptr<Expr> expr) {
  return expr->accept(this);
}

shared_ptr<Object> Interpreter::visitUnaryExpr(Unary expr) {
  shared_ptr<Object> right = evaluate(expr.right);
  shared_ptr<Number> number;

  switch (expr.op.type) {
  case TokenType::BANG:
    return make_shared<Boolean>(!is_truthy(right));
  case TokenType::MINUS:
    check_number_operand(expr.op, right);
    number = dynamic_pointer_cast<Number>(right);
    number->value = -number->value;
    return number;
  default:
    return nullptr;
  }
}

bool Interpreter::is_truthy(shared_ptr<Object> obj) {
  if (obj == nullptr) {
    return false;
  }

  if (typeid(*obj) == typeid(Boolean)) {
    return dynamic_pointer_cast<Boolean>(obj)->value;
  }

  return true;
}

shared_ptr<Object> Interpreter::visitBinaryExpr(Binary expr) {
  shared_ptr<Object> left = evaluate(expr.left);
  shared_ptr<Object> right = evaluate(expr.right);

  switch (expr.op.type) {
  case TokenType::MINUS:
    check_number_operands(expr.op, left, right);
    return make_shared<Number>(dynamic_pointer_cast<Number>(left)->value -
                               dynamic_pointer_cast<Number>(right)->value);
  case TokenType::SLASH:
    check_number_operands(expr.op, left, right);
    return make_shared<Number>(dynamic_pointer_cast<Number>(left)->value /
                               dynamic_pointer_cast<Number>(right)->value);
  case TokenType::STAR:
    check_number_operands(expr.op, left, right);
    return make_shared<Number>(dynamic_pointer_cast<Number>(left)->value *
                               dynamic_pointer_cast<Number>(right)->value);
  case TokenType::PLUS:
    if (typeid(*left) == typeid(Number) && typeid(*right) == typeid(Number)) {
      return make_shared<Number>(dynamic_pointer_cast<Number>(left)->value +
                                 dynamic_pointer_cast<Number>(right)->value);
    }

    if (typeid(*left) == typeid(String) && typeid(*right) == typeid(String)) {
      return make_shared<String>(dynamic_pointer_cast<String>(left)->value +
                                 dynamic_pointer_cast<String>(right)->value);
    }

    throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
  case TokenType::GREATER:
    check_number_operands(expr.op, left, right);
    return make_shared<Boolean>(dynamic_pointer_cast<Number>(left)->value >
                                dynamic_pointer_cast<Number>(right)->value);
  case TokenType::GREATER_EQUAL:
    check_number_operands(expr.op, left, right);
    return make_shared<Boolean>(dynamic_pointer_cast<Number>(left)->value >=
                                dynamic_pointer_cast<Number>(right)->value);
  case TokenType::LESS:
    check_number_operands(expr.op, left, right);
    return make_shared<Boolean>(dynamic_pointer_cast<Number>(left)->value <
                                dynamic_pointer_cast<Number>(right)->value);
  case TokenType::LESS_EQUAL:
    check_number_operands(expr.op, left, right);
    return make_shared<Boolean>(dynamic_pointer_cast<Number>(left)->value <=
                                dynamic_pointer_cast<Number>(right)->value);
  case TokenType::BANG_EQUAL:
    return make_shared<Boolean>(!is_equal(left, right));
  case TokenType::EQUAL_EQUAL:
    return make_shared<Boolean>(is_equal(left, right));
  default:
    return nullptr;
  }
}

bool Interpreter::is_equal(shared_ptr<Object> a, shared_ptr<Object> b) {
  if (a == nullptr && b == nullptr) {
    return true;
  }

  if (a == nullptr) {
    return false;
  }

  return a == b;
}

void Interpreter::check_number_operand(Token op, shared_ptr<Object> operand) {
  if (typeid(*operand) == typeid(Number)) {
    return;
  }

  throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::check_number_operands(Token op, shared_ptr<Object> left,
                                        shared_ptr<Object> right) {

  if (typeid(*left) == typeid(Number) && typeid(*right) == typeid(Number)) {
    return;
  }

  throw RuntimeError(op, "Operands must be numbers.");
}

void Interpreter::interpret(vector<shared_ptr<Stmt>> statements) {
  try {
    for (shared_ptr<Stmt> statement : statements) {
      execute(statement);
    }
  } catch (RuntimeError &error) {
    Vm::runtime_error(error.op, error.message);
  }
}

void Interpreter::execute(shared_ptr<Stmt> stmt) { stmt->accept(this); }

string Interpreter::stringify(shared_ptr<Object> object) {
  if (object == nullptr) {
    return "nil";
  }

  return object->to_string();
}

void Interpreter::visitExpressionStmt(Expression stmt) {
  evaluate(stmt.expr);
  return;
}

void Interpreter::visitPrintStmt(Print stmt) {
  shared_ptr<Object> value = evaluate(stmt.expr);
  cout << stringify(value) << endl;
  return;
}

shared_ptr<Object> Interpreter::visitVariableExpr(Variable expr) {
  return environment->get(expr.name);
}

void Interpreter::visitVarStmt(Var stmt) {
  shared_ptr<Object> value = nullptr;

  if (stmt.initializer != nullptr) {
    value = evaluate(stmt.initializer);
  }

  environment->define(stmt.name.lexeme, value);
  return;
}

shared_ptr<Object> Interpreter::visitAssignExpr(Assign expr) {
  shared_ptr<Object> value = evaluate(expr.value);
  environment->assign(expr.name, value);
  return value;
}

void Interpreter::visitBlockStmt(Block stmt) {
  execute_block(stmt.statements,
                make_shared<Environment>(Environment(environment)));
  return;
}

void Interpreter::execute_block(vector<shared_ptr<Stmt>> statements,
                                shared_ptr<Environment> environment) {
  shared_ptr<Environment> previous = this->environment;

  this->environment = environment;

  for (shared_ptr<Stmt> statement : statements) {
    execute(statement);
  }

  this->environment = previous;
}

void Interpreter::visitIfStmt(If stmt) {
  if (is_truthy(evaluate(stmt.condition))) {
    execute(stmt.then_branch);
  } else if (stmt.else_branch != nullptr) {
    execute(stmt.else_branch);
  }

  return;
}

shared_ptr<Object> Interpreter::visitLogicalExpr(Logical expr) {
  shared_ptr<Object> left = evaluate(expr.left);

  if (expr.op.type == TokenType::OR) {
    if (is_truthy(left))
      return left;
  } else {
    if (!is_truthy(left))
      return left;
  }

  return evaluate(expr.right);
}

void Interpreter::visitWhileStmt(While stmt) {
  while (is_truthy(evaluate(stmt.condition))) {
    execute(stmt.body);
  }

  return;
}
