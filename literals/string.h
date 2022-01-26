#ifndef STRING_H
#define STRING_H

#include "literals/object.h"
#include <string>

class String final : public Object {
public:
  String(std::string value) : value(value) {}
  std::string to_string() const;

private:
  std::string value;
};

#endif
