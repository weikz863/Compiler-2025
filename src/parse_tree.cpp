#include "parse_tree.hpp"
#include <iostream>
#include <algorithm>
#include <ranges>

VisitorBase::~VisitorBase() {}

TreeNode::TreeNode(const EarleyParser &parser, std::size_t end_pos, std::size_t state_pos): 
    data(parser.table[end_pos][state_pos]), children{} {
  const auto &data_ = parser.table[end_pos][state_pos];
  const auto &this_rule = parse_rules[data_.nonterminal_type][data_.production_index];

  std::cerr << "TreeNode type &. : " << data_.nonterminal_type << ' ' << data_.production_index << '\n';

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
        std::cerr << "Nonterminal Type: " << static_cast<int>(std::get<1>(symbol)) << '\n';
        std::size_t best_state_pos = -1;
        for (std::size_t i = 0; i < parser.table[end_pos].size(); i++) {
          const auto &state = parser.table[end_pos][i];
          if (state.nonterminal_type == static_cast<int>(std::get<1>(symbol)) and parser.is_finished(state)) {
            if (best_state_pos == -1 or state.production_index < parser.table[end_pos][best_state_pos].production_index) {
              const auto &prev_states = parser.table[state.start_token_index];
              ParsingState tmp = data_;
              tmp.position_in_production = symbol_index - 1;
              if (auto it = std::find(prev_states.begin(), prev_states.end(), tmp); it != prev_states.end()) {
                best_state_pos = i;
              }
            }
          }
        }
        if (best_state_pos == -1) {
          throw 0;
        } else {
          std::cerr << "next:\n";
          std::cerr << end_pos << ' ' << parser.table[end_pos][best_state_pos].start_token_index << '\n';

          children.push_back(std::make_unique<TreeNode>(parser, end_pos, best_state_pos));
          end_pos = parser.table[end_pos][best_state_pos].start_token_index;

          std::cerr << "updated end_pos: " << end_pos << '\n';
        }
        break;
      }
      default: {
        throw 0;
      }
    }
  }
  std::reverse(children.begin(), children.end());

  std::cerr << "TreeNode type: " << data_.nonterminal_type << " return\n";
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

void PrintVisitor::visit(TreeNode &node) {
  switch(node.data.index()) {
    case 0: { // ParsingState
      break;
    }
    case 1: { // std::string
      std::cout << std::get<1>(node.data) << ' ';
      break;
    }
    default: {
      break;
    }
  }
}