#ifndef VM_H
#define VM_H

#include <fstream>
#include <iostream>
#include <string>

class Vm final {
public:
  static int execute(int argc, char *argv[]);
  static void error(int line, std::string message);

private:
  static bool had_error;

  static int runFile(char *path);
  static void run(std::string source);
  static int runPrompt();
  static void report(int line, std::string where, std::string message);
};

#endif
