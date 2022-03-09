#ifndef NUMBER_H
#define NUMBER_H

#include "literals/object.h"

class Number final : public Object {
public:
  Number(double value) : value(value) {}
  std::string to_string() const;

  double value;

private:
};

#endif
