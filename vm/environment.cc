#include "vm/environment.h"

void Environment::define(string name, shared_ptr<Object> value) {
  values.insert(pair<string, shared_ptr<Object>>(name, value));
}

shared_ptr<Object> Environment::get(Token name) {
  try {
    return values.at(name.lexeme);
  } catch (const std::exception &e) {
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
  }
}