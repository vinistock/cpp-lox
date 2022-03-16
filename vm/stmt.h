#ifndef STMT_H
#define STMT_H

using namespace std;

class Expression;
class Print;

template <class T> class StmtVisitor {
public:
  virtual T visitExpressionStmt(Expression stmt) = 0;
  virtual T visitPrintStmt(Print stmt) = 0;
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

#endif
