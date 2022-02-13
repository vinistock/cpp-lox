#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "literals/string.h"
#include "vm/expr.h"
#include <vector>

using namespace std;

class AstPrinter : public Visitor<String> {
public:
  String print(Expr *expr) { return expr->accept(this); }

  String visitBinaryExpr(Binary expr) {
    vector<Expr *> exprs = {expr.left, expr.right};
    return String(parenthesize(expr.op.lexeme, exprs));
  }

  String visitGroupingExpr(Grouping expr) {
    vector<Expr *> exprs = {expr.expression};
    return String(parenthesize("group", exprs));
  }

  String visitLiteralExpr(Literal expr) {
    if (expr.value == nullptr) {
      return String("nil");
    }

    return String(expr.value->to_string());
  }

  String visitUnaryExpr(Unary expr) {
    vector<Expr *> exprs = {expr.right};
    return String(parenthesize(expr.op.lexeme, exprs));
  }

private:
  string parenthesize(string name, vector<Expr *> exprs) {
    string result = "(";
    result += name;

    for (Expr *expr : exprs) {
      result += " ";
      result += expr->accept(this).to_string();
    }

    result += ")";
    return result;
  }
};

#endif
