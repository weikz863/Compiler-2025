#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include "parse_tree.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
  std::string content {
    std::istreambuf_iterator<char>(std::cin),
    std::istreambuf_iterator<char>()
  }; // another AI told me to use this

  auto tokens = lex(std::string_view(content));

  std::cout << "Tokens:" << std::endl;
  for (const auto& token : tokens) {
    std::cout << "Type: " << static_cast<int>(token.type) << ", Value: '" << token.value << "'" << std::endl;
  }

  try {
    EarleyParser parser(std::move(tokens));
    if (parser.accepts()) {
      // Parse and construct the CST
      auto cst = parser.parse();
      if (cst) {
        std::cout << "Parsing successful! CST constructed." << std::endl;

        // Debug: Print the parse tree structure to file
        std::ofstream out("parse_tree.txt");
        DebugTreeVisitor debugVisitor(out);
        out << "Parse tree structure:" << std::endl;
        cst->accept(debugVisitor);
        out.close();

      } else {
        std::cout << "Parsing failed - unable to construct CST!" << std::endl;
        return 1;
      }
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