#include "parser.hpp"
#include "parse_tree.hpp"
#include <iostream>
#include <algorithm>
#include <optional>

bool ParsingState::operator == (const ParsingState &other) const {
  return std::tie(nonterminal_type, production_index, position_in_production, start_token_index) ==
         std::tie(other.nonterminal_type, other.production_index, other.position_in_production, other.start_token_index);
}
bool ParsingState::operator < (const ParsingState &other) const {
  return std::tie(nonterminal_type, production_index, position_in_production, start_token_index) <
         std::tie(other.nonterminal_type, other.production_index, other.position_in_production, other.start_token_index);
}

// pseodocode from wikipedia

// DECLARE ARRAY S;

// function INIT(words)
//     S ← CREATE_ARRAY(LENGTH(words) + 1)
//     for k ← from 0 to LENGTH(words) do
//         S[k] ← EMPTY_ORDERED_SET

// function EARLEY_PARSE(words, grammar)
//     INIT(words)
//     ADD_TO_SET((γ → •S, 0), S[0])
//     for k ← from 0 to LENGTH(words) do
//         for each state in S[k] do  // S[k] can expand during this loop
//             if not FINISHED(state) then
//                 if NEXT_ELEMENT_OF(state) is a nonterminal then
//                     PREDICTOR(state, k, grammar)         // non_terminal
//                 else do
//                     SCANNER(state, k, words)             // terminal
//             else do
//                 COMPLETER(state, k)
//         end
//     end
//     return chart

// procedure PREDICTOR((A → α•Bβ, j), k, grammar)
//     for each (B → γ) in GRAMMAR_RULES_FOR(B, grammar) do
//         ADD_TO_SET((B → •γ, k), S[k])
//     end

// procedure SCANNER((A → α•aβ, j), k, words)
//     if j < LENGTH(words) and a ⊂ PARTS_OF_SPEECH(words[k]) then
//         ADD_TO_SET((A → αa•β, j), S[k+1])
//     end

// procedure COMPLETER((B → γ•, x), k)
//     for each (A → α•Bβ, j) in S[x] do
//         ADD_TO_SET((A → αB•β, j), S[k])
//     end

bool EarleyParser::is_finished(const ParsingState& state) const {
  const auto& productions = parse_rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];
  return state.position_in_production == production.size();
}

bool EarleyParser::is_empty_production(const ParsingState& state) const {
  const auto& productions = parse_rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];
  return production.empty();
}

Symbol EarleyParser::next_element(const ParsingState& state) const {
  const auto& productions = parse_rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];
  if (state.position_in_production >= production.size()) throw ParseError("next_element called for finished state");
  return production[state.position_in_production];
}

bool EarleyParser::is_nonterminal(const Symbol& symbol) const {
  return std::holds_alternative<Nonterminal>(symbol);
}

bool EarleyParser::is_terminal(const Symbol& symbol) const {
  return std::holds_alternative<Token>(symbol);
}

void EarleyParser::add_to_set(ParsingState state, std::size_t chart_index) {
  auto& chart_set = table[chart_index];
  if (std::find(chart_set.begin(), chart_set.end(), state) == chart_set.end()) {
    chart_set.push_back(state);
  }
}

void EarleyParser::predictor(const ParsingState& state, std::size_t chart_index) {
  auto next = next_element(state);
  if (is_nonterminal(next)) {
    Nonterminal B = std::get<Nonterminal>(next);
    const auto& productions = parse_rules[static_cast<int>(B)];
    for (std::size_t i = 0; i < productions.size(); ++i) {
      ParsingState new_state{
        static_cast<int>(B),
        i,
        0,  // dot at beginning
        chart_index
      };
      add_to_set(new_state, chart_index);
    }
  } else {
    throw ParseError("Predictor - next element is not nonterminal");
  }
}

void EarleyParser::scanner(const ParsingState& state, std::size_t chart_index) {
  if (chart_index < tokens.size()) {
    auto next = next_element(state);
    const Token& expected_token = std::get<Token>(next);
    const Token& current_token = tokens[chart_index];

    // Check if the current token matches using Token::match method
    if (expected_token.match(current_token)) {
      ParsingState new_state = state;
      new_state.position_in_production++;
      add_to_set(new_state, chart_index + 1);
    }
  }
}

void EarleyParser::completer(const ParsingState& state, std::size_t chart_index) {
  if (is_finished(state)) {
    // Find all states in S[state.start_token_index] that were waiting for this nonterminal
    if (state.start_token_index < table.size()) {
      const auto& start_chart = table[state.start_token_index];
      for (const auto& waiting_state : start_chart) {
        if (!is_finished(waiting_state)) {
          auto next_symbol = next_element(waiting_state);
          if (is_nonterminal(next_symbol)) {
            Nonterminal expected_nonterminal = std::get<Nonterminal>(next_symbol);
            if (static_cast<int>(expected_nonterminal) == state.nonterminal_type) {
              ParsingState new_state = waiting_state;
              new_state.position_in_production++;
              add_to_set(new_state, chart_index);
            }
          }
        }
      }
    }
  }
}

EarleyParser::EarleyParser(std::vector<Token>&& input) : tokens{input}, table{tokens.size() + 1} {
  // Add the initial state: ITEMS → •S (start symbol is ITEMS, rule 0)
  ParsingState initial_state{
    static_cast<int>(Nonterminal::ITEMS), // nonterminal_type
    0,                                    // production_index (first production for ITEMS)
    0,                                    // position_in_production (dot at beginning)
    0                                     // start_token_index
  };
  add_to_set(initial_state, 0);

  // Main parsing loop - Earley parser algorithm
  for (std::size_t k = 0; k <= tokens.size(); ++k) {
    // Process all states in S[k] - states can expand during this loop
    for (std::size_t state_index = 0; state_index < table[k].size(); state_index++) {
      const ParsingState state = *std::next(table[k].begin(), state_index);

      if (is_finished(state)) {
        completer(state, k);
      } else if (k < tokens.size()) {
        auto next = next_element(state);
        if (is_nonterminal(next)) {
          predictor(state, k);
        } else if (is_terminal(next)) {
          scanner(state, k);
        }
      }
    }
  }
}

bool EarleyParser::accepts() const {
  // Check if we have a completed parse in the final chart
  if (table.empty()) return false;
  const auto& final_chart = table.back();
  for (const auto& state : final_chart) {
    // Look for a state that represents a completed ITEMS production
    if (state.nonterminal_type == static_cast<int>(Nonterminal::ITEMS) &&
        state.start_token_index == 0 && state.production_index == 0 &&
        is_finished(state)) {
      return true;
    }
  }
  return false;
}

std::unique_ptr<TreeNode> EarleyParser::parse() const {
  if (!accepts()) {
    throw ParseError("Input cannot be parsed");
  }

  // For now, return a simple CST root node to demonstrate the interface works
  return std::make_unique<ItemsNode>();
}

// Helper function to get production length
std::size_t get_production_length(const ParsingState& state) {
  const auto& productions = parse_rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];
  return production.size();
}

// Helper function to create tree nodes for terminals
std::unique_ptr<TreeNode> create_terminal_node(const Token& token) {
  switch (token.type) {
    case Token::Type::Identifier: {
      auto node = std::make_unique<IdentifierNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::Keyword: {
      auto node = std::make_unique<KeywordNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::CharLiteral: {
      auto node = std::make_unique<CharLiteralNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::StringLiteral: {
      auto node = std::make_unique<StringLiteralNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::IntegerLiteral: {
      auto node = std::make_unique<IntegerLiteralNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::Punctuation: {
      auto node = std::make_unique<PunctuationNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::Whitespace: {
      auto node = std::make_unique<WhitespaceNode>();
      node->value = token.value;
      return node;
    }
    case Token::Type::Comment: {
      auto node = std::make_unique<CommentNode>();
      node->value = token.value;
      return node;
    }
    default:
      throw ParseError("Unknown terminal token type");
  }
}

// Helper function to create tree nodes for nonterminals
std::unique_ptr<TreeNode> create_nonterminal_node(Nonterminal nonterminal) {
  switch (nonterminal) {
    // Main nonterminals
    case Nonterminal::ITEMS:
      return std::make_unique<ItemsNode>();
    case Nonterminal::ITEM:
      return std::make_unique<ItemNode>();
    case Nonterminal::FUNCTION:
      return std::make_unique<FunctionNode>();
    case Nonterminal::STRUCT:
      return std::make_unique<StructNode>();
    case Nonterminal::ENUMERATION:
      return std::make_unique<EnumerationNode>();
    case Nonterminal::CONSTANT_ITEM:
      return std::make_unique<ConstantItemNode>();
    case Nonterminal::TRAIT:
      return std::make_unique<TraitNode>();
    case Nonterminal::IMPLEMENTATION:
      return std::make_unique<ImplementationNode>();
    case Nonterminal::INHERENT_IMPL:
      return std::make_unique<InherentImplNode>();
    case Nonterminal::TRAIT_IMPL:
      return std::make_unique<TraitImplNode>();
    case Nonterminal::STATEMENT:
      return std::make_unique<StatementNode>();
    case Nonterminal::LET_STATEMENT:
      return std::make_unique<LetStatementNode>();
    case Nonterminal::EXPRESSION_STATEMENT:
      return std::make_unique<ExpressionStatementNode>();
    case Nonterminal::EXPRESSION:
      return std::make_unique<ExpressionNode>();
    case Nonterminal::BASIC_EXPRESSION:
      return std::make_unique<BasicExpressionNode>();
    case Nonterminal::LITERAL_EXPRESSION:
      return std::make_unique<LiteralExpressionNode>();
    case Nonterminal::UNDERSCORE_EXPRESSION:
      return std::make_unique<UnderscoreExpressionNode>();
    case Nonterminal::GROUPED_EXPRESSION:
      return std::make_unique<GroupedExpressionNode>();
    case Nonterminal::ARRAY_EXPRESSION:
      return std::make_unique<ArrayExpressionNode>();
    case Nonterminal::PATH_EXPRESSION:
      return std::make_unique<PathExpressionNode>();
    case Nonterminal::STRUCT_EXPRESSION:
      return std::make_unique<StructExpressionNode>();
    
    // Additional nodes
    case Nonterminal::OPTIONAL_CONST:
      return std::make_unique<OptionalConstNode>();
      
    // All other nonterminals get stub implementations
    default:
      return std::make_unique<Unused1Node>();
  }
}

// Forward declarations for helper functions
std::unique_ptr<TreeNode> construct_cst(const ParsingState& state, std::size_t i, std::size_t j,
                                       const std::vector<std::vector<ParsingState>>& table,
                                       const std::vector<Token>& tokens);

// Helper function to check if a state is finished (moved outside class)
bool is_finished_state(const ParsingState& state, const std::array<std::vector<Production>, 97>& rules) {
  const auto& productions = rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];
  return state.position_in_production == production.size();
}

// Main parsing function that constructs the CST
std::unique_ptr<TreeNode> construct_cst(const ParsingState& state, std::size_t i, std::size_t j,
                                       const std::vector<std::vector<ParsingState>>& table,
                                       const std::vector<Token>& tokens) {
  const auto& productions = parse_rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];

  // Handle epsilon productions
  if (production.empty()) {
    // For epsilon productions, return appropriate node based on context
    if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_CONST)) {
      auto node = std::make_unique<OptionalConstNode>();
      node->value = ""; // empty string for epsilon
      return node;
    }
    // Add other epsilon production handlers as needed
    return std::make_unique<Unused1Node>();
  }

  // Create the appropriate node for this nonterminal
  auto node = create_nonterminal_node(static_cast<Nonterminal>(state.nonterminal_type));
  
  // Build child nodes based on the production
  std::vector<std::unique_ptr<TreeNode>> children;
  std::size_t token_pos = i;

  for (const auto& symbol : production) {
    if (std::holds_alternative<Token>(symbol)) {
      // Terminal - create terminal node
      const Token& token = std::get<Token>(symbol);
      if (token_pos < tokens.size()) {
        children.push_back(create_terminal_node(tokens[token_pos]));
        token_pos++;
      }
    } else {
      // Nonterminal - recursively build CST
      Nonterminal child_nonterminal = std::get<Nonterminal>(symbol);
      
      // Find the appropriate child state in the parsing table
      bool found_child = false;
      for (std::size_t r = i; r <= j; ++r) {
        if (r >= table.size()) continue;
        
        const auto& chart = table[r];
        for (const auto& child_state : chart) {
          if (child_state.nonterminal_type == static_cast<int>(child_nonterminal) &&
              child_state.start_token_index >= i &&
              child_state.start_token_index + get_production_length(child_state) <= j &&
              child_state.production_index < parse_rules[child_state.nonterminal_type].size() &&
              is_finished_state(child_state, parse_rules)) {
            
            children.push_back(construct_cst(child_state, child_state.start_token_index,
                                           child_state.start_token_index + get_production_length(child_state),
                                           table, tokens));
            found_child = true;
            break;
          }
        }
        if (found_child) break;
      }
      
      if (!found_child) {
        // For some nonterminals that might not have children (like optional ones)
        // This is expected for some epsilon productions
        continue;
      }
    }
  }

  // Set the children in the appropriate node fields
  // This is a simplified implementation - in practice, we'd need to handle each node type specifically
  // based on the production structure and node fields
  
  return node;
}
bool EarleyParser::parse_state(const ParsingState& state, std::size_t i, std::size_t j) const {
  // PARSE(A -> β•, i, j) 
  // /* Finds a rightmost derivation of w[i+1 .. j] starting with production A -> β• */
  //     If β = X1 X2 ... Xm, set k := m and l := j
  //     Repeat until k = 0:
  //         If Xk is terminal:
  //             set k := k - 1 and l := l - 1
  //         Else Xk is nonterminal
  //             for each production Xk -> γ•, tried in order:
  //                 find maximum possible r such that Xk -> γ• in [r, l]
  //                   and A -> X1 X2 ... Xk-1 • Xk ... Xm in [i, r] if exists
  //                 if found, break
  //             Call PARSE(Xk -> γ•, r, l)
  //         Set k := k - 1 and l := r 

  const auto& productions = parse_rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];

  // If β = ε (epsilon production), return true
  if (production.empty()) {
    return true;
  }

  // β = X1 X2 ... Xm, set k := m and l := j
  std::size_t k = production.size() - 1;
  std::size_t l = j;

  // Repeat until k = 0
  while (k < production.size()) {
    const Symbol& symbol = production[k];
    
    if (is_terminal(symbol)) {
      // If Xk is terminal: set k := k - 1 and l := l - 1
      l--;
      if (k == 0) break;
      k--;
    } else {
      // Else Xk is nonterminal
      Nonterminal nonterminal = std::get<Nonterminal>(symbol);
      bool found_subderivation = false;
      
      // for each production Xk -> γ•, tried in order:
      const auto& nonterminal_productions = parse_rules[static_cast<int>(nonterminal)];
      for (std::size_t prod_idx = 0; prod_idx < nonterminal_productions.size(); ++prod_idx) {
        // find maximum possible r such that Xk -> γ• in [r, l]
        for (std::size_t r = l; r >= i && r <= l; --r) {
          if (r >= table.size()) continue;
          
          // and A -> X1 X2 ... Xk-1 • Xk ... Xm in [i, r] if exists
          const auto& chart = table[r];
          for (const auto& test_state : chart) {
            if (test_state.nonterminal_type == static_cast<int>(nonterminal) &&
                test_state.production_index == prod_idx &&
                test_state.start_token_index <= r && 
                r <= test_state.start_token_index + get_production_length(test_state) &&
                is_finished(test_state)) {
              
              // Call PARSE(Xk -> γ•, r, l)
              bool child_result = parse_state(test_state, r, l);
              if (child_result) {
                l = r;
                found_subderivation = true;
                break;
              }
            }
          }
          if (found_subderivation) break;
        }
        if (found_subderivation) break;
      }
      
      if (!found_subderivation) {
        return false;
      }
      
      if (k == 0) break;
      k--;
    }
  }

  // Set k := k - 1 and l := r (already handled in the loop above)
  return true;
}











// pseudocode that ensures a rightmost derivation and correct rule precedence
// adapted from *A Second Course in Formal Languanges and Automata Theory*
// PARSE(A -> β•, i, j) 
// /* Finds a rightmost derivation of w[i+1 .. j] starting with production A -> β• */
//     If β = X1 X2 ... Xm, set k := m and l := j
//     Repeat until k = 0:
//         If Xk is terminal:
//             set k := k - 1 and l := l - 1
//         Else Xk is nonterminal
//             for each production Xk -> γ•, tried in order:
//                 find maximum possible r such that Xk -> γ• in [r, l]
//                   and A -> X1 X2 ... Xk-1 • Xk ... Xm in [i, r] if exists
//                 if found, break
//             Call PARSE(Xk -> γ•, r, l)
//         Set k := k - 1 and l := r 