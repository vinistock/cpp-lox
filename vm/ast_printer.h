#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "literals/string.h"
#include "vm/expr.h"
#include <vector>

using namespace std;

class AstPrinter : public Visitor<String> {
public:
  String print(shared_ptr<Expr> expr) {
    if (expr) {
      return expr->accept(this);
    } else {
      return String("Unhandled expression!");
    }
  }

  String visitBinaryExpr(Binary expr) {
    vector<shared_ptr<Expr>> exprs = {expr.left, expr.right};
    return String(parenthesize(expr.op.lexeme, exprs));
  }

  String visitGroupingExpr(Grouping expr) {
    vector<shared_ptr<Expr>> exprs = {expr.expression};
    return String(parenthesize("group", exprs));
  }

  String visitLiteralExpr(Literal expr) {
    if (expr.value == nullptr) {
      return String("nil");
    }

    return String(expr.value->to_string());
  }

  String visitUnaryExpr(Unary expr) {
    vector<shared_ptr<Expr>> exprs = {expr.right};
    return String(parenthesize(expr.op.lexeme, exprs));
  }

  String visitVariableExpr(Variable expr) { return String(expr.name.lexeme); }

private:
  string parenthesize(string name, vector<shared_ptr<Expr>> exprs) {
    string result = "(";
    result += name;

    for (shared_ptr<Expr> expr : exprs) {
      result += " ";
      result += expr->accept(this).to_string();
    }

    result += ")";
    return result;
  }
};

#endif
