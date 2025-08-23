#include <gtest/gtest.h>
#include "lexer.hpp"

using std::string;

TEST(LexerTest, ReadCharacters) {
  auto [c1, len1] = read_characters("a");
  EXPECT_EQ(c1, 'a');
  EXPECT_EQ(len1, 1);

  auto [c2, len2] = read_characters("\\n");
  EXPECT_EQ(c2, '\n');
  EXPECT_EQ(len2, 2);

  auto [c3, len3] = read_characters("\\t");
  EXPECT_EQ(c3, '\t');
  EXPECT_EQ(len3, 2);

  auto [c4, len4] = read_characters("\\'");
  EXPECT_EQ(c4, '\'');
  EXPECT_EQ(len4, 2);

  auto [c5, len5] = read_characters("\\\"");
  EXPECT_EQ(c5, '"');
  EXPECT_EQ(len5, 2);

  auto [c6, len6] = read_characters("\\\\");
  EXPECT_EQ(c6, '\\');
  EXPECT_EQ(len6, 2);

  auto [c7, len7] = read_characters("b");
  EXPECT_EQ(c7, 'b');
  EXPECT_EQ(len7, 1);
}

TEST(LexerTest, KeywordLex) {
  auto tokens = lex("let");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::Keyword);
  EXPECT_EQ(token.value, "let");
}

TEST(LexerTest, IdentifierLex) {
  auto tokens = lex("x");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::Identifier);
  EXPECT_EQ(token.value, "x");
}

TEST(LexerTest, IntegerLiteralLex) {
  auto tokens = lex("42");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::IntegerLiteral);
  EXPECT_EQ(token.value, "42");
}

TEST(LexerTest, PunctuationLex) {
  auto tokens = lex("=");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::Punctuation);
  EXPECT_EQ(token.value, "=");
}

TEST(LexerTest, StringLiteralLex) {
  auto tokens = lex("\"hello, world!\"");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::StringLiteral);
  EXPECT_EQ(token.value, "\"hello, world!\"");
}

TEST(LexerTest, CharLiteralLex) {
  auto tokens = lex("'a'");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::CharLiteral);
  EXPECT_EQ(token.value, "'a'");
}

TEST(LexerTest, EscapedCharLiteralLex) {
  auto tokens = lex("'\\n'");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::CharLiteral);
  EXPECT_EQ(token.value, "'\\n'");
}

TEST(LexerTest, EscapedCharLiteralLex2) {
  auto tokens = lex("'\"'");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::CharLiteral);
  EXPECT_EQ(token.value, "'\"'");
}

TEST(LexerTest, EscapedCharLiteralLex3) {
  auto tokens = lex(R"("'\"\'\\")");
  ASSERT_EQ(tokens.size(), 1);
  auto token = tokens[0];
  EXPECT_EQ(token.type, Token::Type::StringLiteral);
  EXPECT_EQ(token.value, R"("'\"\'\\")");
}

TEST(LexerTest, CompLex) {
  string input = R"(let x = 42;
if x > 10 {
  println!("x is greater than 10");
} else {
  println!("x is less than or equal to 10");
}
)";
  auto tokens = lex(input);
  std::vector<string> expected_values = {
    "let", "x", "=", "42", ";",
    "if", "x", ">", "10", "{",
    "println", "!", "(", "\"x is greater than 10\"", ")", ";",
    "}", "else", "{",
    "println", "!", "(", "\"x is less than or equal to 10\"", ")", ";",
    "}"
  };
  std::vector<Token::Type> expected_types = {
    Token::Type::Keyword, Token::Type::Identifier, Token::Type::Punctuation, Token::Type::IntegerLiteral, Token::Type::Punctuation,
    Token::Type::Keyword, Token::Type::Identifier, Token::Type::Punctuation, Token::Type::IntegerLiteral, Token::Type::Punctuation,
    Token::Type::Identifier, Token::Type::Punctuation, Token::Type::Punctuation, Token::Type::StringLiteral, Token::Type::Punctuation, Token::Type::Punctuation,
    Token::Type::Punctuation, Token::Type::Keyword, Token::Type::Punctuation,
    Token::Type::Identifier, Token::Type::Punctuation, Token::Type::Punctuation, Token::Type::StringLiteral, Token::Type::Punctuation, Token::Type::Punctuation,
    Token::Type::Punctuation
  };
  ASSERT_EQ(tokens.size(), expected_types.size());
  ASSERT_EQ(tokens.size(), expected_values.size());
  for (size_t i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].value, expected_values[i]);
    EXPECT_EQ(tokens[i].type, expected_types[i]);
  }
}