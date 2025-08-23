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