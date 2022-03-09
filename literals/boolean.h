#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "literals/object.h"

class Boolean final : public Object {
public:
  Boolean(bool value) : value(value) {}
  std::string to_string() const;

  bool value;
};

#endif
