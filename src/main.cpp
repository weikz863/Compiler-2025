#include <iostream>
#include <string>
#include <string_view>
#include "lexer.hpp"
#include "parser.hpp"

int main() {
  std::string content(
    std::istreambuf_iterator<char>(std::cin),
    std::istreambuf_iterator<char>()
  ); // another AI told me to use this
  auto tokens = lex(std::string_view(content));
  EarleyParser parser(std::move(tokens));
}