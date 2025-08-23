#pragma once

#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <string_view>
#include <vector>

class Token;
class LexerError;

std::pair<char, int> read_characters(const std::string_view &);
std::vector<Token> lex(const std::string_view &);

#endif