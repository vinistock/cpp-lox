#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object {
public:
  virtual std::string to_string() const = 0;
};

#endif
