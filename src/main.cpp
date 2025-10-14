#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
  std::string content;

  try {
    if (argc > 1) {
      // Read from file
      std::ifstream file(argv[1]);
      if (!file) {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
      }
      content.assign(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
      );

      // Limit debug output for very large inputs
      if (content.size() > 100) {
        std::cerr << "Input: '(large input - " << content.size() << " characters)'" << std::endl;
      } else {
        std::cerr << "Input: '" << content << "'" << std::endl;
      }
    } else {
      // Check if stdin is empty first
      if (std::cin.peek() == EOF) {
        std::cerr << "Input: '(empty)'" << std::endl;
        content = "";
      } else {
        content.assign(
          std::istreambuf_iterator<char>(std::cin),
          std::istreambuf_iterator<char>()
        ); // another AI told me to use this

        // Limit debug output for very large inputs
        if (content.size() > 100) {
          std::cerr << "Input: '(large input - " << content.size() << " characters)'" << std::endl;
        } else {
          std::cerr << "Input: '" << content << "'" << std::endl;
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error reading input: " << e.what() << std::endl;
    return 1;
  }

  auto tokens = lex(std::string_view(content));

  // std::cerr << "Tokens:" << std::endl;
  // for (const auto& token : tokens) {
  //   std::cerr << "  " << static_cast<int>(token.type) << ": '" << token.value << "'" << std::endl;
  // }

  try {
    std::cout << "Creating parser..." << std::endl;
    EarleyParser parser(std::move(tokens));

    std::cout << "Parser created, checking acceptance..." << std::endl;
    // Test if parsing was successful
    if (parser.accepts()) {
      std::cout << "Parsing successful!" << std::endl;
      return 0;
    } else {
      std::cout << "Parsing failed!" << std::endl;
      std::cout << "Tokens (" << tokens.size() << "):" << std::endl;
      for (size_t i = 0; i < tokens.size(); ++i) {
        std::cout << "  " << i << ": " << static_cast<int>(tokens[i].type) << " '" << tokens[i].value << "'" << std::endl;
      }
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