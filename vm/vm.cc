#include "vm/vm.h"

bool Vm::had_error = false;
bool Vm::had_runtime_error = false;

int Vm::execute(int argc, char *argv[]) {
  if (argc > 2) {
    std::cout << "Usage: vini-lox [script]" << std::endl;
    return 64;
  } else if (argc == 2) {
    return Vm::runFile(argv[1]);
  } else {
    Vm::runPrompt();
  }

  return 0;
}

void Vm::run(std::string source) {
  Scanner scanner = Scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();

  Parser parser = Parser(tokens);
  vector<shared_ptr<Stmt>> statements = parser.parse();

  if (Vm::had_error) {
    return;
  }

  Interpreter interpreter = Interpreter();
  interpreter.interpret(statements);
  return;
}

int Vm::runFile(char *path) {
  std::string source, line;
  std::ifstream input_file(path);

  while (getline(input_file, line)) {
    source += line;
  }

  input_file.close();

  Vm::run(source);

  if (Vm::had_error) {
    return 65;
  }

  if (Vm::had_runtime_error) {
    return 70;
  }

  return 0;
}

int Vm::runPrompt() {
  std::string source;

  while (true) {
    std::cout << "> ";
    std::getline(std::cin, source);

    if (source == "quit" || source == "q")
      break;

    Vm::run(source);
    Vm::had_error = false;
  }

  return 0;
}

void Vm::error(int line, std::string message) {
  Vm::report(line, "", message);
  return;
}

void Vm::error(Token token, std::string message) {
  if (token.type == TokenType::ENDOF) {
    Vm::report(token.line, " at end", message);
  } else {
    Vm::report(token.line, " at '" + token.lexeme + "'", message);
  }

  return;
}

void Vm::report(int line, std::string where, std::string message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message
            << std::endl;
  Vm::had_error = true;
  return;
}

void Vm::runtime_error(Token op, std::string message) {
  cout << message << "\n[line " << op.line << "]" << endl;
  Vm::had_runtime_error = true;
}
