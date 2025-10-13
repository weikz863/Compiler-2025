#include "parser.hpp"
#include <map>

bool ParsingState::operator == (const ParsingState &other) const {
  return std::tie(nonterminal_type, production_index, position_in_production, start_token_index) ==
         std::tie(other.nonterminal_type, other.production_index, other.position_in_production, other.start_token_index);
}
bool ParsingState::operator < (const ParsingState &other) const {
  return std::tie(nonterminal_type, production_index, position_in_production, start_token_index) <
         std::tie(other.nonterminal_type, other.production_index, other.position_in_production, other.start_token_index);
}

EarleyParser::EarleyParser(std::vector<Token>&& input) : tokens{input}, table{} {
}