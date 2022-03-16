#ifndef ERRORS_H
#define ERRORS_H

#include "vm/token.h"

using namespace std;

class RuntimeError : public exception {
public:
  RuntimeError(Token op, const string &message) : op(op), message(message) {}

  Token op;
  string message;
};

#endif
