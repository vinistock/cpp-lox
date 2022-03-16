#ifndef VM_H
#define VM_H

#include "vm/ast_printer.h"
#include "vm/interpreter.h"
#include "vm/parser.h"
#include "vm/scanner.h"
#include "vm/stmt.h"
#include "vm/token.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Vm final {
public:
  static int execute(int argc, char *argv[]);
  static void error(int line, std::string message);
  static void error(Token token, std::string message);
  static void runtime_error(Token op, std::string message);

private:
  static bool had_error;
  static bool had_runtime_error;

  static int runFile(char *path);
  static void run(std::string source);
  static int runPrompt();
  static void report(int line, std::string where, std::string message);
};

#endif
