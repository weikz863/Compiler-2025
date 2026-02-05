#include "parser.hpp"
#include "parse_tree.hpp"
#include <iostream>
#include <algorithm>
#include <optional>

// Forward declarations for helper functions
std::unique_ptr<TreeNode> construct_cst(const ParsingState& state, std::size_t i, std::size_t j,
                                        const std::vector<std::vector<ParsingState>>& table,
                                        const std::vector<Token>& tokens, int depth = 0);

// Helper function to check if a state is finished (moved outside class)
bool is_finished_state(const ParsingState& state, const std::array<std::vector<Production>, 97>& rules) {
  const auto& productions = rules[state.nonterminal_type];
  const auto& production = productions[state.production_index];
  return state.position_in_production == production.size();
}

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

  // Find the completed ITEMS state in the final chart
  const auto& final_chart = table.back();
  for (const auto& state : final_chart) {
    if (state.nonterminal_type == static_cast<int>(Nonterminal::ITEMS) &&
        state.start_token_index == 0 && state.production_index == 0 &&
        is_finished(state)) {
      // Construct the CST using the completed parse state
      return construct_cst(state, state.start_token_index, tokens.size(), table, tokens, 0);
    }
  }
  
  // Fallback - should not reach here if accepts() returned true
  throw ParseError("Unable to construct CST despite successful parse");
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
    case Nonterminal::OPTIONAL_CONST:
      return std::make_unique<OptionalConstNode>();
    case Nonterminal::FUNCTION_PARAMETERS:
      return std::make_unique<FunctionParametersNode>();
    case Nonterminal::SELF_PARAM:
      return std::make_unique<SelfParamNode>();
    case Nonterminal::SHORTHAND_SELF:
      return std::make_unique<ShorthandSelfNode>();
    case Nonterminal::TYPED_SELF:
      return std::make_unique<TypedSelfNode>();
    case Nonterminal::FUNCTION_PARAM:
      return std::make_unique<FunctionParamNode>();
    case Nonterminal::FUNCTION_RETURN_TYPE:
      return std::make_unique<FunctionReturnTypeNode>();
    case Nonterminal::OPTIONAL_FUNCTION_PARAMETERS:
      return std::make_unique<OptionalFunctionParametersNode>();
    case Nonterminal::OPTIONAL_COMMA:
      return std::make_unique<OptionalCommaNode>();
    case Nonterminal::COMMA_FUNCTION_PARAMS:
      return std::make_unique<CommaFunctionParamsNode>();
    case Nonterminal::OPTIONAL_FUNCTION_RETURN_TYPE:
      return std::make_unique<OptionalFunctionReturnTypeNode>();
    case Nonterminal::BLOCK_EXPRESSION_OR_SEMICOLON:
      return std::make_unique<BlockExpressionOrSemicolonNode>();
    case Nonterminal::STRUCT:
      return std::make_unique<StructNode>();
    case Nonterminal::STRUCT_FIELDS:
      return std::make_unique<StructFieldsNode>();
    case Nonterminal::STRUCT_FIELD:
      return std::make_unique<StructFieldNode>();
    case Nonterminal::OPTIONAL_STRUCT_FIELDS:
      return std::make_unique<OptionalStructFieldsNode>();
    case Nonterminal::COMMA_STRUCT_FIELDS:
      return std::make_unique<CommaStructFieldsNode>();
    case Nonterminal::ENUMERATION:
      return std::make_unique<EnumerationNode>();
    case Nonterminal::ENUM_VARIANTS:
      return std::make_unique<EnumVariantsNode>();
    case Nonterminal::ENUM_VARIANT:
      return std::make_unique<EnumVariantNode>();
    case Nonterminal::OPTIONAL_ENUM_VARIANTS:
      return std::make_unique<OptionalEnumVariantsNode>();
    case Nonterminal::COMMA_ENUM_VARIANTS:
      return std::make_unique<CommaEnumVariantsNode>();
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
    case Nonterminal::UNUSED1:
      return std::make_unique<Unused1Node>();
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
    case Nonterminal::OPTIONAL_ARRAY_ELEMENTS:
      return std::make_unique<OptionalArrayElementsNode>();
    case Nonterminal::ARRAY_ELEMENTS:
      return std::make_unique<ArrayElementsNode>();
    case Nonterminal::COMMA_ARRAY_ELEMENTS:
      return std::make_unique<CommaArrayElementsNode>();
    case Nonterminal::PATH_EXPRESSION:
      return std::make_unique<PathExpressionNode>();
    case Nonterminal::STRUCT_EXPRESSION:
      return std::make_unique<StructExpressionNode>();
    case Nonterminal::OPTIONAL_STRUCT_EXPR_FIELDS:
      return std::make_unique<OptionalStructExprFieldsNode>();
    case Nonterminal::STRUCT_EXPR_FIELDS:
      return std::make_unique<StructExprFieldsNode>();
    case Nonterminal::COMMA_STRUCT_EXPR_FIELDS:
      return std::make_unique<CommaStructExprFieldsNode>();
    case Nonterminal::STRUCT_EXPR_FIELD:
      return std::make_unique<StructExprFieldNode>();
    case Nonterminal::POSTFIX_EXPRESSION:
      return std::make_unique<PostfixExpressionNode>();
    case Nonterminal::METHOD_CALL_EXPRESSION:
      return std::make_unique<MethodCallExpressionNode>();
    case Nonterminal::OPTIONAL_CALL_PARAMS:
      return std::make_unique<OptionalCallParamsNode>();
    case Nonterminal::CALL_PARAMS:
      return std::make_unique<CallParamsNode>();
    case Nonterminal::COMMA_CALL_PARAMS:
      return std::make_unique<CommaCallParamsNode>();
    case Nonterminal::FIELD_EXPRESSION:
      return std::make_unique<FieldExpressionNode>();
    case Nonterminal::CALL_EXPRESSION:
      return std::make_unique<CallExpressionNode>();
    case Nonterminal::INDEX_EXPRESSION:
      return std::make_unique<IndexExpressionNode>();
    case Nonterminal::UNARY_OPERATOR_EXPRESSION:
      return std::make_unique<UnaryOperatorExpressionNode>();
    case Nonterminal::BORROW_EXPRESSION:
      return std::make_unique<BorrowExpressionNode>();
    case Nonterminal::DEREFERENCE_EXPRESSION:
      return std::make_unique<DereferenceExpressionNode>();
    case Nonterminal::NEGATION_EXPRESSION:
      return std::make_unique<NegationExpressionNode>();
    case Nonterminal::TYPE_CAST_EXPRESSION:
      return std::make_unique<TypeCastExpressionNode>();
    case Nonterminal::MULTIPLICATIVE_OPERATOR_EXPRESSION:
      return std::make_unique<MultiplicativeOperatorExpressionNode>();
    case Nonterminal::ADDITIVE_OPERATOR_EXPRESSION:
      return std::make_unique<AdditiveOperatorExpressionNode>();
    case Nonterminal::SHIFT_OPERATOR_EXPRESSION:
      return std::make_unique<ShiftOperatorExpressionNode>();
    case Nonterminal::AND_EXPRESSION:
      return std::make_unique<AndExpressionNode>();
    case Nonterminal::XOR_EXPRESSION:
      return std::make_unique<XorExpressionNode>();
    case Nonterminal::OR_EXPRESSION:
      return std::make_unique<OrExpressionNode>();
    case Nonterminal::COMPARISON_OPERATOR_EXPRESSION:
      return std::make_unique<ComparisonOperatorExpressionNode>();
    case Nonterminal::LAZY_AND_EXPRESSION:
      return std::make_unique<LazyAndExpressionNode>();
    case Nonterminal::LAZY_OR_EXPRESSION:
      return std::make_unique<LazyOrExpressionNode>();
    case Nonterminal::ASSIGNMENT_EXPRESSION:
      return std::make_unique<AssignmentExpressionNode>();
    case Nonterminal::SIMPLE_ASSIGNMENT_EXPRESSION:
      return std::make_unique<SimpleAssignmentExpressionNode>();
    case Nonterminal::COMPOUND_ASSIGNMENT_EXPRESSION:
      return std::make_unique<CompoundAssignmentExpressionNode>();
    case Nonterminal::FLOW_CONTROL_EXPRESSION:
      return std::make_unique<FlowControlExpressionNode>();
    case Nonterminal::CONTINUE_EXPRESSION:
      return std::make_unique<ContinueExpressionNode>();
    case Nonterminal::BREAK_EXPRESSION:
      return std::make_unique<BreakExpressionNode>();
    case Nonterminal::RETURN_EXPRESSION:
      return std::make_unique<ReturnExpressionNode>();
    case Nonterminal::EXPRESSION_WITH_BLOCK:
      return std::make_unique<ExpressionWithBlockNode>();
    case Nonterminal::BLOCK_EXPRESSION:
      return std::make_unique<BlockExpressionNode>();
    case Nonterminal::STATEMENTS:
      return std::make_unique<StatementsNode>();
    case Nonterminal::LOOP_EXPRESSION:
      return std::make_unique<LoopExpressionNode>();
    case Nonterminal::INFINITE_LOOP_EXPRESSION:
      return std::make_unique<InfiniteLoopExpressionNode>();
    case Nonterminal::PREDICATE_LOOP_EXPRESSION:
      return std::make_unique<PredicateLoopExpressionNode>();
    case Nonterminal::IF_EXPRESSION:
      return std::make_unique<IfExpressionNode>();
    case Nonterminal::CONDITIONS:
      return std::make_unique<ConditionsNode>();
    case Nonterminal::PATTERN:
      return std::make_unique<PatternNode>();
    case Nonterminal::IDENTIFIER_PATTERN:
      return std::make_unique<IdentifierPatternNode>();
    case Nonterminal::WILDCARD_PATTERN:
      return std::make_unique<WildcardPatternNode>();
    case Nonterminal::REFERENCE_PATTERN:
      return std::make_unique<ReferencePatternNode>();
    case Nonterminal::TYPE:
      return std::make_unique<TypeNode>();
    case Nonterminal::TYPE_PATH:
      return std::make_unique<TypePathNode>();
    case Nonterminal::REFERENCE_TYPE:
      return std::make_unique<ReferenceTypeNode>();
    case Nonterminal::ARRAY_TYPE:
      return std::make_unique<ArrayTypeNode>();
    case Nonterminal::UNIT_TYPE:
      return std::make_unique<UnitTypeNode>();
    case Nonterminal::PATH_IN_EXPRESSION:
      return std::make_unique<PathInExpressionNode>();
    case Nonterminal::PATH_EXPR_SEGMENT:
      return std::make_unique<PathExprSegmentNode>();
      
    // All other nonterminals get stub implementations
    default:
      return std::make_unique<Unused1Node>();
  }
}

// Main parsing function that constructs the CST
std::unique_ptr<TreeNode> construct_cst(const ParsingState& state, std::size_t i, std::size_t j,
                                         const std::vector<std::vector<ParsingState>>& table,
                                         const std::vector<Token>& tokens, int depth) {
    if (i > j || depth > 100) return std::make_unique<Unused1Node>();

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
      // For ITEMS epsilon production, return empty ItemsNode
      if (state.nonterminal_type == static_cast<int>(Nonterminal::ITEMS)) {
        auto node = std::make_unique<ItemsNode>();
        return node;
      }
      // Add other epsilon production handlers as needed
      return std::make_unique<Unused1Node>();
    }

    // Create the appropriate node for this nonterminal
    auto node = create_nonterminal_node(static_cast<Nonterminal>(state.nonterminal_type));

   // Special handling for ITEMS
   if (state.nonterminal_type == static_cast<int>(Nonterminal::ITEMS)) {
     auto items_node = static_cast<ItemsNode*>(node.get());

     if (state.production_index == 0) { // ITEMS -> ITEMS ITEM
       // Find the split point k where ITEMS ends at k and ITEM starts at k
       for (std::size_t k = i; k <= j; ++k) {
         bool has_items = (k == i); // epsilon ITEMS
         if (k > i && k < table.size()) {
           has_items = std::any_of(table[k].begin(), table[k].end(), [&](const ParsingState& s) {
             return s.nonterminal_type == static_cast<int>(Nonterminal::ITEMS) &&
                    s.start_token_index == i && is_finished_state(s, parse_rules);
           });
         }
         bool has_item = (k <= j && j < table.size()) ? std::any_of(table[j].begin(), table[j].end(), [&](const ParsingState& s) {
           return s.nonterminal_type == static_cast<int>(Nonterminal::ITEM) &&
                  s.start_token_index == k && is_finished_state(s, parse_rules);
         }) : false;

         if (has_items && has_item) {
           if (k > i) {
             // Add the ITEMS child
             auto items_it = std::find_if(table[k].begin(), table[k].end(), [&](const ParsingState& s) {
               return s.nonterminal_type == static_cast<int>(Nonterminal::ITEMS) &&
                      s.start_token_index == i && is_finished_state(s, parse_rules);
             });
             if (items_it != table[k].end()) {
               items_node->items.push_back(construct_cst(*items_it, i, k, table, tokens, depth + 1));
             }
           }
           // Add the ITEM child
           auto item_it = std::find_if(table[j].begin(), table[j].end(), [&](const ParsingState& s) {
             return s.nonterminal_type == static_cast<int>(Nonterminal::ITEM) &&
                    s.start_token_index == k && is_finished_state(s, parse_rules);
           });
           if (item_it != table[j].end()) {
             items_node->items.push_back(construct_cst(*item_it, k, j, table, tokens, depth + 1));
           }
           break;
         }
       }
     }
     // For epsilon production, do nothing (empty items)
     return node;
   }

   // Build child nodes by iterating through the production symbols
   std::size_t token_pos = i;
   std::size_t symbol_index = 0;

   for (const auto& symbol : production) {
     if (std::holds_alternative<Token>(symbol)) {
       // Terminal symbol - consume token and create terminal node
       const Token& expected_token = std::get<Token>(symbol);
       if (token_pos < tokens.size() && expected_token.match(tokens[token_pos])) {
         // Create terminal node and add to children
         auto terminal_node = create_terminal_node(tokens[token_pos]);
         node->children.push_back(std::move(terminal_node));
         token_pos++;
       }
       symbol_index++;
     } else if (std::holds_alternative<Nonterminal>(symbol)) {
       // Nonterminal symbol - find the child state and recursively construct
       Nonterminal child_nonterminal = std::get<Nonterminal>(symbol);

       // Find finished state for this nonterminal starting at token_pos
       std::optional<std::pair<ParsingState, std::size_t>> found;
       for (int chart_k = j; chart_k >= (int)token_pos; --chart_k) {
         if ((std::size_t)chart_k >= table.size()) continue;
         const auto& chart = table[chart_k];
         for (const auto& child_state : chart) {
           if (child_state.nonterminal_type == static_cast<int>(child_nonterminal) &&
               child_state.start_token_index == token_pos &&
               is_finished_state(child_state, parse_rules)) {
             if (!found || (std::size_t)chart_k > found->second) {
               found = {child_state, (std::size_t)chart_k};
             }
           }
         }
       }
       if (found) {
         auto [child_state, chart_k] = *found;
         auto child_node = construct_cst(child_state, token_pos, chart_k, table, tokens, depth + 1);
         node->children.push_back(std::move(child_node));
         token_pos = chart_k;
       } else {
         node->children.push_back(create_nonterminal_node(child_nonterminal));
       }
       symbol_index++;
     }
   }

   // Set specific fields for nodes that have specific child fields
   if (state.nonterminal_type == static_cast<int>(Nonterminal::ITEM)) {
     auto item_node = static_cast<ItemNode*>(node.get());
     if (!node->children.empty()) {
       item_node->item = node->children[0].get();
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::FUNCTION)) {
     auto fn_node = static_cast<FunctionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 8) {
       fn_node->optional_const = node->children[0].get();
       // children[1] is "fn", children[2] is Identifier
       fn_node->identifier = static_cast<IdentifierNode*>(node->children[2].get())->value;
       // children[3] is "(", children[4] is OPTIONAL_FUNCTION_PARAMETERS
       fn_node->optional_function_parameters = node->children[4].get();
       // children[5] is ")", children[6] is OPTIONAL_FUNCTION_RETURN_TYPE
       fn_node->optional_function_return_type = node->children[6].get();
       // children[7] is BLOCK_EXPRESSION_OR_SEMICOLON
       fn_node->block_expression_or_semicolon = node->children[7].get();
     }
     // node->children.clear(); // don't clear to keep ownership
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::FUNCTION_PARAMETERS)) {
     auto fp_node = static_cast<FunctionParametersNode*>(node.get());
     if (state.production_index == 0) {
       fp_node->self_param = node->children[0].get();
       fp_node->optional_comma = node->children[1].get();
     } else if (state.production_index == 1) {
       fp_node->function_params.push_back(node->children[0].get());
       fp_node->comma_function_params = node->children[1].get();
       fp_node->optional_comma = node->children[2].get();
     } else if (state.production_index == 2) {
       fp_node->self_param = node->children[0].get();
       fp_node->function_params.push_back(node->children[2].get());
       fp_node->comma_function_params = node->children[3].get();
       fp_node->optional_comma = node->children[4].get();
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_CONST)) {
     auto oc_node = static_cast<OptionalConstNode*>(node.get());
     if (state.production_index == 0) {
       oc_node->value = "const";
     } else {
       oc_node->value = "";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::SELF_PARAM)) {
     auto sp_node = static_cast<SelfParamNode*>(node.get());
     if (state.production_index == 0) {
       sp_node->shorthand_self = node->children[0].get();
     } else {
       sp_node->typed_self = node->children[0].get();
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::SHORTHAND_SELF)) {
     auto ss_node = static_cast<ShorthandSelfNode*>(node.get());
     if (state.production_index == 0) {
       ss_node->ampersand = "&";
       ss_node->mut = "mut";
       ss_node->self = "self";
     } else if (state.production_index == 1) {
       ss_node->ampersand = "&";
       ss_node->self = "self";
     } else if (state.production_index == 2) {
       ss_node->mut = "mut";
       ss_node->self = "self";
     } else {
       ss_node->self = "self";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::TYPED_SELF)) {
     auto ts_node = static_cast<TypedSelfNode*>(node.get());
     if (state.production_index == 0) {
       ts_node->mut = "mut";
       ts_node->self = "self";
       ts_node->type = std::move(node->children[3]);
     } else {
       ts_node->self = "self";
       ts_node->type = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::FUNCTION_PARAM)) {
     auto fparam_node = static_cast<FunctionParamNode*>(node.get());
     fparam_node->pattern = std::move(node->children[0]);
     fparam_node->type = std::move(node->children[2]);
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::FUNCTION_RETURN_TYPE)) {
     auto frt_node = static_cast<FunctionReturnTypeNode*>(node.get());
     frt_node->type = std::move(node->children[1]);
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_FUNCTION_PARAMETERS)) {
     auto ofp_node = static_cast<OptionalFunctionParametersNode*>(node.get());
     if (state.production_index == 0) {
       ofp_node->function_parameters = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_FUNCTION_RETURN_TYPE)) {
     auto ofrt_node = static_cast<OptionalFunctionReturnTypeNode*>(node.get());
     if (state.production_index == 0) {
       ofrt_node->function_return_type = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_COMMA)) {
     auto oc_node = static_cast<OptionalCommaNode*>(node.get());
     if (state.production_index == 0) {
       oc_node->value = ",";
     } else {
       oc_node->value = "";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_FUNCTION_PARAMS)) {
     auto cfp_node = static_cast<CommaFunctionParamsNode*>(node.get());
     if (state.production_index == 0) {
       auto prev = static_cast<CommaFunctionParamsNode*>(node->children[0].get());
       cfp_node->function_params = prev->function_params;
       cfp_node->function_params.push_back(node->children[2].get());
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::BLOCK_EXPRESSION_OR_SEMICOLON)) {
     auto beos_node = static_cast<BlockExpressionOrSemicolonNode*>(node.get());
     if (state.production_index == 0) {
       beos_node->block_expression = std::move(node->children[0]);
       beos_node->semicolon = "";
     } else {
       beos_node->semicolon = static_cast<PunctuationNode*>(node->children[0].get())->value;
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::BLOCK_EXPRESSION)) {
     auto be_node = static_cast<BlockExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       be_node->statements = node->children[1].get();
       be_node->expression = node->children[2].get();
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       be_node->statements = node->children[1].get();
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STATEMENTS)) {
     auto st_node = static_cast<StatementsNode*>(node.get());
     if (state.production_index == 0) {
       if (node->children.size() >= 3) {
         auto prev_st = static_cast<StatementsNode*>(node->children[0].get());
         st_node->statements = prev_st->statements;
         st_node->statements.push_back(node->children[2].get());
       } else if (node->children.size() >= 1) {
         st_node->statements.push_back(node->children[0].get());
       }
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::EXPRESSION)) {
     auto expr_node = static_cast<ExpressionNode*>(node.get());
     if (state.production_index == 0) {
       expr_node->flow_control_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::FLOW_CONTROL_EXPRESSION)) {
     auto fce_node = static_cast<FlowControlExpressionNode*>(node.get());
     if (state.production_index == 0) {
       fce_node->assignment_expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       fce_node->continue_expression = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       fce_node->break_expression = std::move(node->children[0]);
     } else if (state.production_index == 3) {
       fce_node->return_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ASSIGNMENT_EXPRESSION)) {
     auto ae_node = static_cast<AssignmentExpressionNode*>(node.get());
     if (state.production_index == 0) {
       ae_node->lazy_or_expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       ae_node->simple_assignment_expression = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       ae_node->compound_assignment_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::SIMPLE_ASSIGNMENT_EXPRESSION)) {
     auto sae_node = static_cast<SimpleAssignmentExpressionNode*>(node.get());
     sae_node->lazy_or_expression = std::move(node->children[0]);
     sae_node->assignment_expression = std::move(node->children[2]);
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMPOUND_ASSIGNMENT_EXPRESSION)) {
     auto cae_node = static_cast<CompoundAssignmentExpressionNode*>(node.get());
     cae_node->lazy_or_expression = std::move(node->children[0]);
     cae_node->operator_ = static_cast<PunctuationNode*>(node->children[1].get())->value;
     cae_node->assignment_expression = std::move(node->children[2]);
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::LAZY_OR_EXPRESSION)) {
     auto lor_node = static_cast<LazyOrExpressionNode*>(node.get());
     if (state.production_index == 0) {
       lor_node->lazy_and_expression = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       lor_node->lazy_and_expression = std::move(node->children[0]);
       lor_node->lazy_or_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::LAZY_AND_EXPRESSION)) {
     auto land_node = static_cast<LazyAndExpressionNode*>(node.get());
     if (state.production_index == 0) {
       land_node->comparison_operator_expression = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       land_node->comparison_operator_expression = std::move(node->children[0]);
       land_node->lazy_and_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMPARISON_OPERATOR_EXPRESSION)) {
     auto coe_node = static_cast<ComparisonOperatorExpressionNode*>(node.get());
     if (state.production_index == 0) {
       coe_node->or_expression = std::move(node->children[0]);
     } else if (state.production_index >= 1 && node->children.size() >= 3) {
       coe_node->or_expression = std::move(node->children[0]);
       coe_node->operator_ = static_cast<PunctuationNode*>(node->children[1].get())->value;
       coe_node->or_expression_right = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OR_EXPRESSION)) {
     auto or_node = static_cast<OrExpressionNode*>(node.get());
     if (state.production_index == 0) {
       or_node->xor_expression = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       or_node->xor_expression = std::move(node->children[0]);
       or_node->or_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::XOR_EXPRESSION)) {
     auto xor_node = static_cast<XorExpressionNode*>(node.get());
     if (state.production_index == 0) {
       xor_node->and_expression = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       xor_node->and_expression = std::move(node->children[0]);
       xor_node->xor_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::AND_EXPRESSION)) {
     auto and_node = static_cast<AndExpressionNode*>(node.get());
     if (state.production_index == 0) {
       and_node->shift_operator_expression = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       and_node->shift_operator_expression = std::move(node->children[0]);
       and_node->and_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::SHIFT_OPERATOR_EXPRESSION)) {
     auto soe_node = static_cast<ShiftOperatorExpressionNode*>(node.get());
     if (state.production_index == 0) {
       soe_node->additive_operator_expression = std::move(node->children[0]);
     } else if (state.production_index >= 1 && node->children.size() >= 3) {
       soe_node->additive_operator_expression = std::move(node->children[0]);
       soe_node->operator_ = static_cast<PunctuationNode*>(node->children[1].get())->value;
       soe_node->shift_operator_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ADDITIVE_OPERATOR_EXPRESSION)) {
     auto aoe_node = static_cast<AdditiveOperatorExpressionNode*>(node.get());
     if (state.production_index == 0) {
       aoe_node->multiplicative_operator_expression = std::move(node->children[0]);
     } else if (state.production_index >= 1 && node->children.size() >= 3) {
       aoe_node->multiplicative_operator_expression = std::move(node->children[0]);
       aoe_node->operator_ = static_cast<PunctuationNode*>(node->children[1].get())->value;
       aoe_node->additive_operator_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::MULTIPLICATIVE_OPERATOR_EXPRESSION)) {
     auto moe_node = static_cast<MultiplicativeOperatorExpressionNode*>(node.get());
     if (state.production_index == 0) {
       moe_node->type_cast_expression = std::move(node->children[0]);
     } else if (state.production_index >= 1 && node->children.size() >= 3) {
       moe_node->type_cast_expression = std::move(node->children[0]);
       moe_node->operator_ = static_cast<PunctuationNode*>(node->children[1].get())->value;
       moe_node->multiplicative_operator_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::TYPE_CAST_EXPRESSION)) {
     auto tce_node = static_cast<TypeCastExpressionNode*>(node.get());
     if (state.production_index == 0) {
       tce_node->unary_operator_expression = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       tce_node->unary_operator_expression = std::move(node->children[0]);
       tce_node->type_cast_expression = std::move(node->children[1]);
       tce_node->type = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::UNARY_OPERATOR_EXPRESSION)) {
     auto uoe_node = static_cast<UnaryOperatorExpressionNode*>(node.get());
     if (state.production_index == 0) {
       uoe_node->postfix_expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       uoe_node->borrow_expression = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       uoe_node->dereference_expression = std::move(node->children[0]);
     } else if (state.production_index == 3) {
       uoe_node->negation_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::POSTFIX_EXPRESSION)) {
     auto pe_node = static_cast<PostfixExpressionNode*>(node.get());
     if (state.production_index == 0) {
       pe_node->basic_expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       pe_node->method_call_expression = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       pe_node->field_expression = std::move(node->children[0]);
     } else if (state.production_index == 3) {
       pe_node->call_expression = std::move(node->children[0]);
     } else if (state.production_index == 4) {
       pe_node->index_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::BASIC_EXPRESSION)) {
     auto be_node = static_cast<BasicExpressionNode*>(node.get());
     if (state.production_index == 0) {
       be_node->literal_expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       be_node->underscore_expression = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       be_node->grouped_expression = std::move(node->children[0]);
     } else if (state.production_index == 3) {
       be_node->array_expression = std::move(node->children[0]);
     } else if (state.production_index == 4) {
       be_node->path_expression = std::move(node->children[0]);
     } else if (state.production_index == 5) {
       be_node->struct_expression = std::move(node->children[0]);
     } else if (state.production_index == 6) {
       be_node->expression_with_block = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::LITERAL_EXPRESSION)) {
     auto le_node = static_cast<LiteralExpressionNode*>(node.get());
     if (state.production_index == 0) {
       le_node->char_literal = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       le_node->string_literal = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       le_node->integer_literal = std::move(node->children[0]);
     } else if (state.production_index == 3) {
       le_node->true_keyword = "true";
     } else if (state.production_index == 4) {
       le_node->false_keyword = "false";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PATH_EXPRESSION)) {
     auto pe_node = static_cast<PathExpressionNode*>(node.get());
     if (state.production_index == 0) {
       pe_node->path_in_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::UNIT_TYPE)) {
     // UnitTypeNode has no fields
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PATH_IN_EXPRESSION)) {
     auto pie_node = static_cast<PathInExpressionNode*>(node.get());
     if (state.production_index == 0) {
       pie_node->path_expr_segment = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       pie_node->path_expr_segment = std::move(node->children[0]);
       pie_node->path_expr_segment2 = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PATH_EXPR_SEGMENT)) {
     auto pes_node = static_cast<PathExprSegmentNode*>(node.get());
     if (state.production_index == 0) {
       pes_node->identifier = static_cast<IdentifierNode*>(node->children[0].get())->value;
     } else if (state.production_index == 1) {
       pes_node->self_keyword = "Self";
     } else if (state.production_index == 2) {
       pes_node->self_keyword = "self";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::CALL_EXPRESSION)) {
     auto ce_node = static_cast<CallExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       ce_node->postfix_expression = std::move(node->children[0]);
       ce_node->optional_call_params = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_CALL_PARAMS)) {
     auto ocp_node = static_cast<OptionalCallParamsNode*>(node.get());
     if (state.production_index == 0) {
       ocp_node->call_params = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::CALL_PARAMS)) {
     auto cp_node = static_cast<CallParamsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       cp_node->expressions.push_back(std::move(node->children[0]));
       auto ccp = static_cast<CommaCallParamsNode*>(node->children[1].get());
       cp_node->expressions.insert(cp_node->expressions.end(), std::make_move_iterator(ccp->expressions.begin()), std::make_move_iterator(ccp->expressions.end()));
       cp_node->optional_comma = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_CALL_PARAMS)) {
     auto ccp_node = static_cast<CommaCallParamsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       auto prev_ccp = static_cast<CommaCallParamsNode*>(node->children[0].get());
       ccp_node->expressions = std::move(prev_ccp->expressions);
       ccp_node->expressions.push_back(std::move(node->children[2]));
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::NEGATION_EXPRESSION)) {
     auto ne_node = static_cast<NegationExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       ne_node->operator_ = static_cast<PunctuationNode*>(node->children[0].get())->value;
       ne_node->unary_operator_expression = std::move(node->children[1]);
     } else if (state.production_index == 1 && node->children.size() >= 2) {
       ne_node->operator_ = static_cast<PunctuationNode*>(node->children[0].get())->value;
       ne_node->unary_operator_expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT)) {
     auto struct_node = static_cast<StructNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 5) {
       struct_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       struct_node->optional_struct_fields = std::move(node->children[3]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       struct_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       struct_node->semicolon = ";";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT_FIELDS)) {
     auto sf_node = static_cast<StructFieldsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       sf_node->struct_fields.push_back(std::move(node->children[0]));
       auto csf = static_cast<CommaStructFieldsNode*>(node->children[1].get());
       sf_node->struct_fields.insert(sf_node->struct_fields.end(), std::make_move_iterator(csf->struct_fields.begin()), std::make_move_iterator(csf->struct_fields.end()));
       sf_node->optional_comma = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT_FIELD)) {
     auto sf_node = static_cast<StructFieldNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       sf_node->identifier = static_cast<IdentifierNode*>(node->children[0].get())->value;
       sf_node->type = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_STRUCT_FIELDS)) {
     auto osf_node = static_cast<OptionalStructFieldsNode*>(node.get());
     if (state.production_index == 0) {
       osf_node->struct_fields = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_STRUCT_FIELDS)) {
     auto csf_node = static_cast<CommaStructFieldsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       auto prev_csf = static_cast<CommaStructFieldsNode*>(node->children[0].get());
       csf_node->struct_fields = std::move(prev_csf->struct_fields);
       csf_node->struct_fields.push_back(std::move(node->children[2]));
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ENUMERATION)) {
     auto enum_node = static_cast<EnumerationNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       enum_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       enum_node->optional_enum_variants = std::move(node->children[3]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ENUM_VARIANTS)) {
     auto ev_node = static_cast<EnumVariantsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       ev_node->enum_variants.push_back(std::move(node->children[0]));
       auto cev = static_cast<CommaEnumVariantsNode*>(node->children[1].get());
       ev_node->enum_variants.insert(ev_node->enum_variants.end(), std::make_move_iterator(cev->enum_variants.begin()), std::make_move_iterator(cev->enum_variants.end()));
       ev_node->optional_comma = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ENUM_VARIANT)) {
     auto ev_node = static_cast<EnumVariantNode*>(node.get());
     if (state.production_index == 0) {
       ev_node->identifier = static_cast<IdentifierNode*>(node->children[0].get())->value;
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_ENUM_VARIANTS)) {
     auto oev_node = static_cast<OptionalEnumVariantsNode*>(node.get());
     if (state.production_index == 0) {
       oev_node->enum_variants = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_ENUM_VARIANTS)) {
     auto cev_node = static_cast<CommaEnumVariantsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       auto prev_cev = static_cast<CommaEnumVariantsNode*>(node->children[0].get());
       cev_node->enum_variants = std::move(prev_cev->enum_variants);
       cev_node->enum_variants.push_back(std::move(node->children[2]));
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::CONSTANT_ITEM)) {
     auto ci_node = static_cast<ConstantItemNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 6) {
       ci_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       ci_node->type = std::move(node->children[3]);
       ci_node->expression = std::move(node->children[5]);
     } else if (state.production_index == 1 && node->children.size() >= 5) {
       ci_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       ci_node->type = std::move(node->children[3]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::TRAIT)) {
     auto trait_node = static_cast<TraitNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       trait_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       trait_node->items = std::move(node->children[3]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::IMPLEMENTATION)) {
     auto impl_node = static_cast<ImplementationNode*>(node.get());
     if (state.production_index == 0) {
       impl_node->inherent_impl = std::move(node->children[0]);
     } else {
       impl_node->trait_impl = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::INHERENT_IMPL)) {
     auto ii_node = static_cast<InherentImplNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       ii_node->type = std::move(node->children[1]);
       ii_node->items = std::move(node->children[3]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::TRAIT_IMPL)) {
     auto ti_node = static_cast<TraitImplNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 6) {
       ti_node->identifier = static_cast<IdentifierNode*>(node->children[1].get())->value;
       ti_node->type = std::move(node->children[3]);
       ti_node->items = std::move(node->children[5]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STATEMENT)) {
     auto stmt_node = static_cast<StatementNode*>(node.get());
     if (state.production_index == 0) {
       stmt_node->semicolon = ";";
     } else if (state.production_index == 1) {
       stmt_node->item = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       stmt_node->let_statement = std::move(node->children[0]);
     } else if (state.production_index == 3) {
       stmt_node->expression_statement = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::LET_STATEMENT)) {
     auto ls_node = static_cast<LetStatementNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 6) {
       ls_node->pattern = std::move(node->children[1]);
       ls_node->type = std::move(node->children[3]);
       ls_node->expression = std::move(node->children[5]);
     } else if (state.production_index == 1 && node->children.size() >= 5) {
       ls_node->pattern = std::move(node->children[1]);
       ls_node->type = std::move(node->children[3]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::EXPRESSION_STATEMENT)) {
     auto es_node = static_cast<ExpressionStatementNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       es_node->expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       es_node->expression_with_block = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::UNDERSCORE_EXPRESSION)) {
     auto ue_node = static_cast<UnderscoreExpressionNode*>(node.get());
     if (state.production_index == 0) {
       ue_node->value = "_";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::GROUPED_EXPRESSION)) {
     auto ge_node = static_cast<GroupedExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       ge_node->expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ARRAY_EXPRESSION)) {
     auto ae_node = static_cast<ArrayExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       ae_node->optional_array_elements = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_ARRAY_ELEMENTS)) {
     auto oae_node = static_cast<OptionalArrayElementsNode*>(node.get());
     if (state.production_index == 0) {
       oae_node->array_elements = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ARRAY_ELEMENTS)) {
     auto ae_node = static_cast<ArrayElementsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       ae_node->expressions.push_back(std::move(node->children[0]));
       auto cae = static_cast<CommaArrayElementsNode*>(node->children[1].get());
       ae_node->expressions.insert(ae_node->expressions.end(), std::make_move_iterator(cae->expressions.begin()), std::make_move_iterator(cae->expressions.end()));
       ae_node->optional_comma = std::move(node->children[2]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       ae_node->expression = std::move(node->children[0]);
       ae_node->size_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_ARRAY_ELEMENTS)) {
     auto cae_node = static_cast<CommaArrayElementsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       auto prev_cae = static_cast<CommaArrayElementsNode*>(node->children[0].get());
       cae_node->expressions = std::move(prev_cae->expressions);
       cae_node->expressions.push_back(std::move(node->children[2]));
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT_EXPRESSION)) {
     auto se_node = static_cast<StructExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       se_node->path_in_expression = std::move(node->children[0]);
       se_node->optional_struct_expr_fields = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_STRUCT_EXPR_FIELDS)) {
     auto osef_node = static_cast<OptionalStructExprFieldsNode*>(node.get());
     if (state.production_index == 0) {
       osef_node->struct_expr_fields = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT_EXPR_FIELDS)) {
     auto sef_node = static_cast<StructExprFieldsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       sef_node->struct_expr_fields.push_back(std::move(node->children[0]));
       auto csef = static_cast<CommaStructExprFieldsNode*>(node->children[1].get());
       sef_node->struct_expr_fields.insert(sef_node->struct_expr_fields.end(), std::make_move_iterator(csef->struct_expr_fields.begin()), std::make_move_iterator(csef->struct_expr_fields.end()));
       sef_node->optional_comma = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_STRUCT_EXPR_FIELDS)) {
     auto csef_node = static_cast<CommaStructExprFieldsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       auto prev_csef = static_cast<CommaStructExprFieldsNode*>(node->children[0].get());
       csef_node->struct_expr_fields = std::move(prev_csef->struct_expr_fields);
       csef_node->struct_expr_fields.push_back(std::move(node->children[2]));
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT_EXPR_FIELD)) {
     auto sef_node = static_cast<StructExprFieldNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       sef_node->identifier = static_cast<IdentifierNode*>(node->children[0].get())->value;
       sef_node->expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::METHOD_CALL_EXPRESSION)) {
     auto mce_node = static_cast<MethodCallExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 5) {
       mce_node->postfix_expression = std::move(node->children[0]);
       mce_node->path_expr_segment = std::move(node->children[2]);
       mce_node->optional_call_params = std::move(node->children[4]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::FIELD_EXPRESSION)) {
     auto fe_node = static_cast<FieldExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       fe_node->postfix_expression = std::move(node->children[0]);
       fe_node->identifier = static_cast<IdentifierNode*>(node->children[2].get())->value;
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::INDEX_EXPRESSION)) {
     auto ie_node = static_cast<IndexExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       ie_node->postfix_expression = std::move(node->children[0]);
       ie_node->expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::BORROW_EXPRESSION)) {
     auto be_node = static_cast<BorrowExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       be_node->ampersand = "&";
       be_node->mut = "mut";
       be_node->unary_operator_expression = std::move(node->children[2]);
     } else if (state.production_index == 1 && node->children.size() >= 2) {
       be_node->ampersand = "&";
       be_node->unary_operator_expression = std::move(node->children[1]);
     } else if (state.production_index == 2 && node->children.size() >= 3) {
       be_node->ampersand = "&&";
       be_node->mut = "mut";
       be_node->unary_operator_expression = std::move(node->children[2]);
     } else if (state.production_index == 3 && node->children.size() >= 2) {
       be_node->ampersand = "&&";
       be_node->unary_operator_expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::DEREFERENCE_EXPRESSION)) {
     auto de_node = static_cast<DereferenceExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       de_node->unary_operator_expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::EXPRESSION_WITH_BLOCK)) {
     auto ewb_node = static_cast<ExpressionWithBlockNode*>(node.get());
     if (state.production_index == 0) {
       ewb_node->block_expression = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       ewb_node->loop_expression = std::move(node->children[0]);
     } else {
       ewb_node->if_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::CONTINUE_EXPRESSION)) {
     auto ce_node = static_cast<ContinueExpressionNode*>(node.get());
     if (state.production_index == 0) {
       ce_node->value = "continue";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::BREAK_EXPRESSION)) {
     auto be_node = static_cast<BreakExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       be_node->flow_control_expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::RETURN_EXPRESSION)) {
     auto re_node = static_cast<ReturnExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       re_node->flow_control_expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::LOOP_EXPRESSION)) {
     auto le_node = static_cast<LoopExpressionNode*>(node.get());
     if (state.production_index == 0) {
       le_node->infinite_loop_expression = std::move(node->children[0]);
     } else {
       le_node->predicate_loop_expression = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::INFINITE_LOOP_EXPRESSION)) {
     auto ile_node = static_cast<InfiniteLoopExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       ile_node->block_expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PREDICATE_LOOP_EXPRESSION)) {
     auto ple_node = static_cast<PredicateLoopExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       ple_node->conditions = std::move(node->children[1]);
       ple_node->block_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::IF_EXPRESSION)) {
     auto ie_node = static_cast<IfExpressionNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 5) {
       ie_node->conditions = std::move(node->children[1]);
       ie_node->block_expression = std::move(node->children[2]);
       ie_node->else_if_expression = std::move(node->children[4]);
     } else if (state.production_index == 1 && node->children.size() >= 5) {
       ie_node->conditions = std::move(node->children[1]);
       ie_node->block_expression = std::move(node->children[2]);
       ie_node->else_block_expression = std::move(node->children[4]);
     } else if (state.production_index == 2 && node->children.size() >= 3) {
       ie_node->conditions = std::move(node->children[1]);
       ie_node->block_expression = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::CONDITIONS)) {
     auto c_node = static_cast<ConditionsNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       c_node->expression = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PATTERN)) {
     auto p_node = static_cast<PatternNode*>(node.get());
     if (state.production_index == 0) {
       p_node->identifier_pattern = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       p_node->wildcard_pattern = std::move(node->children[0]);
     } else {
       p_node->reference_pattern = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::IDENTIFIER_PATTERN)) {
     auto ip_node = static_cast<IdentifierPatternNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 4) {
       ip_node->ref = "ref";
       ip_node->mut = "mut";
       ip_node->identifier = static_cast<IdentifierNode*>(node->children[3].get())->value;
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       ip_node->ref = "ref";
       ip_node->identifier = static_cast<IdentifierNode*>(node->children[2].get())->value;
     } else if (state.production_index == 2 && node->children.size() >= 3) {
       ip_node->mut = "mut";
       ip_node->identifier = static_cast<IdentifierNode*>(node->children[2].get())->value;
     } else if (state.production_index == 3 && node->children.size() >= 1) {
       ip_node->identifier = static_cast<IdentifierNode*>(node->children[0].get())->value;
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::WILDCARD_PATTERN)) {
     auto wp_node = static_cast<WildcardPatternNode*>(node.get());
     if (state.production_index == 0) {
       wp_node->value = "_";
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::REFERENCE_PATTERN)) {
     auto rp_node = static_cast<ReferencePatternNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 3) {
       rp_node->ampersand = "&";
       rp_node->mut = "mut";
       rp_node->pattern = std::move(node->children[2]);
     } else if (state.production_index == 1 && node->children.size() >= 2) {
       rp_node->ampersand = "&";
       rp_node->pattern = std::move(node->children[1]);
     } else if (state.production_index == 2 && node->children.size() >= 3) {
       rp_node->ampersand = "&&";
       rp_node->mut = "mut";
       rp_node->pattern = std::move(node->children[2]);
     } else if (state.production_index == 3 && node->children.size() >= 2) {
       rp_node->ampersand = "&&";
       rp_node->pattern = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::TYPE)) {
     auto t_node = static_cast<TypeNode*>(node.get());
     if (state.production_index == 0) {
       t_node->type_path = std::move(node->children[0]);
     } else if (state.production_index == 1) {
       t_node->reference_type = std::move(node->children[0]);
     } else if (state.production_index == 2) {
       t_node->array_type = std::move(node->children[0]);
     } else {
       t_node->unit_type = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::TYPE_PATH)) {
     auto tp_node = static_cast<TypePathNode*>(node.get());
     if (state.production_index == 0) {
       tp_node->path_expr_segment = std::move(node->children[0]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::REFERENCE_TYPE)) {
     auto rt_node = static_cast<ReferenceTypeNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 2) {
       rt_node->mut = "mut";
       rt_node->type = std::move(node->children[1]);
     } else if (state.production_index == 1 && node->children.size() >= 2) {
       rt_node->type = std::move(node->children[1]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::ARRAY_TYPE)) {
     auto at_node = static_cast<ArrayTypeNode*>(node.get());
     if (state.production_index == 0 && node->children.size() >= 5) {
       at_node->type = std::move(node->children[1]);
       at_node->expression = std::move(node->children[3]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PATH_IN_EXPRESSION)) {
     auto pie_node = static_cast<PathInExpressionNode*>(node.get());
     if (state.production_index == 0) {
       pie_node->path_expr_segment = std::move(node->children[0]);
     } else if (state.production_index == 1 && node->children.size() >= 3) {
       pie_node->path_expr_segment = std::move(node->children[0]);
       pie_node->path_expr_segment2 = std::move(node->children[2]);
     }
   } else if (state.nonterminal_type == static_cast<int>(Nonterminal::PATH_EXPR_SEGMENT)) {
     auto pes_node = static_cast<PathExprSegmentNode*>(node.get());
     if (state.production_index == 0) {
       pes_node->identifier = static_cast<IdentifierNode*>(node->children[0].get())->value;
     } else if (state.production_index == 1) {
       pes_node->self_keyword = "Self";
     } else if (state.production_index == 2) {
       pes_node->self_keyword = "self";
     }
   }

   return node;
}
