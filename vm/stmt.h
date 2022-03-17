#ifndef STMT_H
#define STMT_H

#include <memory>
#include <vector>

using namespace std;

class Expression;
class Print;
class Var;
class Block;
class If;

template <class T> class StmtVisitor {
public:
  virtual T visitExpressionStmt(Expression stmt) = 0;
  virtual T visitPrintStmt(Print stmt) = 0;
  virtual T visitVarStmt(Var stmt) = 0;
  virtual T visitBlockStmt(Block stmt) = 0;
  virtual T visitIfStmt(If stmt) = 0;
};

class Stmt {
public:
  virtual void accept(StmtVisitor<void> *visitor) = 0;
};

class Expression final : public Stmt {
public:
  Expression(shared_ptr<Expr> expr) : expr(expr) {}

  void accept(StmtVisitor<void> *visitor) {
    return visitor->visitExpressionStmt(*this);
  }

  shared_ptr<Expr> expr;
};

class Print final : public Stmt {
public:
  Print(shared_ptr<Expr> expr) : expr(expr) {}

  void accept(StmtVisitor<void> *visitor) {
    return visitor->visitPrintStmt(*this);
  }

  shared_ptr<Expr> expr;
};

class Var final : public Stmt {
public:
  Var(Token name, shared_ptr<Expr> initializer)
      : name(name), initializer(initializer) {}

  void accept(StmtVisitor<void> *visitor) {
    return visitor->visitVarStmt(*this);
  }

  Token name;
  shared_ptr<Expr> initializer;
};

class Block final : public Stmt {
public:
  Block(vector<shared_ptr<Stmt>> statements) : statements(statements) {}

  void accept(StmtVisitor<void> *visitor) {
    return visitor->visitBlockStmt(*this);
  }

  vector<shared_ptr<Stmt>> statements;
};

class If final : public Stmt {
public:
  If(shared_ptr<Expr> condition, shared_ptr<Stmt> then_branch,
     shared_ptr<Stmt> else_branch)
      : condition(condition), then_branch(then_branch),
        else_branch(else_branch) {}

  void accept(StmtVisitor<void> *visitor) {
    return visitor->visitIfStmt(*this);
  }

  shared_ptr<Expr> condition;
  shared_ptr<Stmt> then_branch;
  shared_ptr<Stmt> else_branch;
};

#endif
