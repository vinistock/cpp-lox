#include "vm/environment.h"

void Environment::define(string name, shared_ptr<Object> value) {
  values.insert(pair<string, shared_ptr<Object>>(name, value));
}

shared_ptr<Object> Environment::get(Token name) {
  try {
    return values.at(name.lexeme);
  } catch (const std::exception &e) {
    if (enclosing != nullptr) {
      return enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
  }
}

void Environment::assign(Token name, shared_ptr<Object> value) {
  try {
    values.at(name.lexeme) = value;
  } catch (const std::exception &e) {
    if (enclosing != nullptr) {
      enclosing->assign(name, value);
    } else {
      throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
  }
}
