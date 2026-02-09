#include "parse_tree.hpp"
#include <algorithm>
#include <ranges>

VisitorBase::~VisitorBase() {}

TreeNode::TreeNode(const EarleyParser &parser, std::size_t end_pos, std::size_t state_pos): 
    data(parser.table[end_pos][state_pos]), children{} {
  const auto &data_ = parser.table[end_pos][state_pos];
  const auto &this_rule = parse_rules[data_.nonterminal_type][data_.production_index];
  for (std::size_t symbol_index = this_rule.size(); symbol_index > 0; symbol_index--) {
    const auto &symbol = this_rule[symbol_index - 1];
    switch (symbol.index()) {
      case 0: { // Token
        if (not std::get<0>(symbol).match(parser.tokens[end_pos - 1])) throw 0;
        children.push_back(std::make_unique<TreeNode>(parser.tokens[end_pos - 1].value));
        end_pos--;
        break;
      }
      case 1: { // Nonterminal
        std::size_t best_state_pos = -1;
        for (const auto &state: parser.table[end_pos]) {
          if (state.nonterminal_type == static_cast<int>(std::get<1>(symbol))) {
            const auto &prev_states = parser.table[state.start_token_index];
            ParsingState tmp = data_;
            tmp.position_in_production = symbol_index - 1;
            if (auto it = std::find(prev_states.begin(), prev_states.end(), tmp); it != prev_states.end()) {
              if (best_state_pos == -1 or it->production_index < parser.table[end_pos][best_state_pos].production_index) {
                best_state_pos = it - prev_states.begin();
              }
            }
          }
        }
        if (best_state_pos == -1) {
          throw 0;
        } else {
          children.push_back(std::make_unique<TreeNode>(parser, end_pos, best_state_pos));
          end_pos = best_state_pos;
        }
        break;
      }
      default: {
        throw 0;
      }
    }
  }
  std::reverse(children.begin(), children.end());
}

TreeNode::TreeNode(const std::string &str): data(str), children{} {}

void TreeNode::accept(VisitorBase &visitor) {
  for (const auto &child: this->children) {
    if (child) {
      child->accept(visitor);
    }
  }
  visitor.visit(*this);
}