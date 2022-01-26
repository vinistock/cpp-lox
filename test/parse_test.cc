#include "vm/scanner.h"
#include "vm/token.h"
#include <memory>

int assert_tokens(std::string message, std::string source,
                  std::vector<TokenType> types,
                  std::vector<std::shared_ptr<Object>> values) {
  Scanner scanner = Scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();
  bool keep_going = true;
  int i = 0;

  // Compare the size of the tokens (ignoring EOF)
  if (types.size() != tokens.size() - 1) {
    std::cout << "Number of token types does not match" << std::endl;
    return 1;
  }

  while (i < types.size() && keep_going) {
    if (types[i] != tokens[i].type) {
      std::cout << "Token types are not equal" << std::endl;
      keep_going = false;
    }

    i++;
  }

  if (!keep_going)
    return 1;

  i = 0;

  while (i < values.size() && keep_going) {
    if (values[i]->to_string() != tokens[i].literal->to_string()) {
      std::cout << "Token values are not equal" << std::endl;
      keep_going = false;
    }

    i++;
  }

  if (!keep_going)
    return 1;

  return 0;
}

int main() {
  std::vector<TokenType> types;
  std::vector<std::shared_ptr<Object>> values;

  types = {TokenType::STRING};
  values = {std::make_shared<String>("a string")};
  if (assert_tokens("Test parsing strings", "\"a string\"", types, values))
    return 1;

  types = {TokenType::CLASS, TokenType::IDENTIFIER, TokenType::LEFT_BRACE,
           TokenType::RIGHT_BRACE};
  values = {};
  if (assert_tokens("Test parsing class definition", "class Something {\n}",
                    types, values))
    return 1;

  types = {TokenType::NUMBER};
  values = {std::make_shared<Number>(5.5)};
  if (assert_tokens("Test parsing numbers", "5.5", types, values))
    return 1;

  types = {};
  values = {};
  if (assert_tokens("Test comments", "// some comment", types, values))
    return 1;

  return 0;
}
