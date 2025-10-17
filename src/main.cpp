#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
  std::string content {
    std::istreambuf_iterator<char>(std::cin),
    std::istreambuf_iterator<char>()
  }; // another AI told me to use this

  auto tokens = lex(std::string_view(content));

  try {
    EarleyParser parser(std::move(tokens));
    if (parser.accepts()) {
      std::cout << "Parsing successful!" << std::endl;
    } else {
      std::cout << "Parsing failed!" << std::endl;
      return 1;
    }
  } catch (const std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cout << "Unknown exception occurred!" << std::endl;
    return 1;
  }
}