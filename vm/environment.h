#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "literals/object.h"
#include "vm/errors.h"
#include "vm/token.h"
#include <map>

using namespace std;

class Environment final {
public:
  void define(string name, shared_ptr<Object> value);
  void assign(Token name, shared_ptr<Object> value);
  shared_ptr<Object> get(Token name);

private:
  map<string, shared_ptr<Object>> values;
};

#endif
