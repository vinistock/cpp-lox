#ifndef NUMBER_H
#define NUMBER_H

#include "vm/object.h"

class Number final : public Object {
public:
  Number(double value) : value(value) {}
  std::string to_string() const;

private:
  double value;
};

#endif
