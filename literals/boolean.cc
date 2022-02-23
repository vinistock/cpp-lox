#include "literals/boolean.h"

std::string Boolean::to_string() const { return value ? "true" : "false"; }
