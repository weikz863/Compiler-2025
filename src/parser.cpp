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
  
  // Build child nodes and link them based on the production
  std::size_t token_pos = i;
  std::size_t production_index = state.production_index;

  // Handle specific node types based on nonterminal type and production
  switch (static_cast<Nonterminal>(state.nonterminal_type)) {
    case Nonterminal::ITEMS: {
      auto items_node = static_cast<ItemsNode*>(node.get());
      if (production_index == 0) { // ITEMS -> ITEMS ITEM
        // First child is ITEMS, second is ITEM
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::ITEMS) &&
                is_finished_state(child_state, parse_rules)) {
              items_node->items.push_back(construct_cst(child_state, child_state.start_token_index,
                                                       child_state.start_token_index + get_production_length(child_state),
                                                       table, tokens));
              break;
            }
          }
        }
        // Add ITEM
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::ITEM) &&
                is_finished_state(child_state, parse_rules)) {
              items_node->items.push_back(construct_cst(child_state, child_state.start_token_index,
                                                       child_state.start_token_index + get_production_length(child_state),
                                                       table, tokens));
              break;
            }
          }
        }
      }
      break;
    }
    
    case Nonterminal::ITEM: {
      auto item_node = static_cast<ItemNode*>(node.get());
      // Single child - the actual item (FUNCTION, STRUCT, etc.)
      for (const auto& symbol : production) {
        if (std::holds_alternative<Nonterminal>(symbol)) {
          Nonterminal child_nonterminal = std::get<Nonterminal>(symbol);
          for (std::size_t r = i; r <= j; ++r) {
            if (r >= table.size()) continue;
            const auto& chart = table[r];
            for (const auto& child_state : chart) {
              if (child_state.nonterminal_type == static_cast<int>(child_nonterminal) &&
                  is_finished_state(child_state, parse_rules)) {
                item_node->item = construct_cst(child_state, child_state.start_token_index,
                                               child_state.start_token_index + get_production_length(child_state),
                                               table, tokens);
                break;
              }
            }
          }
        }
      }
      break;
    }
    
    case Nonterminal::FUNCTION: {
      auto func_node = static_cast<FunctionNode*>(node.get());
      // OPTIONAL_CONST "fn" Identifier "(" OPTIONAL_FUNCTION_PARAMETERS ")" OPTIONAL_FUNCTION_RETURN_TYPE BLOCK_EXPRESSION_OR_SEMICOLON
      std::size_t symbol_idx = 0;
      
      // Handle OPTIONAL_CONST
      if (symbol_idx < production.size() && std::holds_alternative<Nonterminal>(production[symbol_idx]) &&
          std::get<Nonterminal>(production[symbol_idx]) == Nonterminal::OPTIONAL_CONST) {
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_CONST) &&
                is_finished_state(child_state, parse_rules)) {
              func_node->optional_const = construct_cst(child_state, child_state.start_token_index,
                                                       child_state.start_token_index + get_production_length(child_state),
                                                       table, tokens);
              break;
            }
          }
        }
        symbol_idx++;
      }
      
      // Skip "fn" keyword
      symbol_idx++;
      
      // Handle Identifier
      if (symbol_idx < production.size() && std::holds_alternative<Token>(production[symbol_idx])) {
        const Token& identifier_token = std::get<Token>(production[symbol_idx]);
        if (identifier_token.type == Token::Type::Identifier && token_pos < tokens.size()) {
          func_node->identifier = tokens[token_pos].value;
          token_pos++;
        }
        symbol_idx++;
      }
      
      // Skip "("
      symbol_idx++;
      
      // Handle OPTIONAL_FUNCTION_PARAMETERS
      if (symbol_idx < production.size() && std::holds_alternative<Nonterminal>(production[symbol_idx]) &&
          std::get<Nonterminal>(production[symbol_idx]) == Nonterminal::OPTIONAL_FUNCTION_PARAMETERS) {
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_FUNCTION_PARAMETERS) &&
                is_finished_state(child_state, parse_rules)) {
              func_node->optional_function_parameters = construct_cst(child_state, child_state.start_token_index,
                                                                     child_state.start_token_index + get_production_length(child_state),
                                                                     table, tokens);
              break;
            }
          }
        }
        symbol_idx++;
      }
      
      // Skip ")"
      symbol_idx++;
      
      // Handle OPTIONAL_FUNCTION_RETURN_TYPE
      if (symbol_idx < production.size() && std::holds_alternative<Nonterminal>(production[symbol_idx]) &&
          std::get<Nonterminal>(production[symbol_idx]) == Nonterminal::OPTIONAL_FUNCTION_RETURN_TYPE) {
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_FUNCTION_RETURN_TYPE) &&
                is_finished_state(child_state, parse_rules)) {
              func_node->optional_function_return_type = construct_cst(child_state, child_state.start_token_index,
                                                                      child_state.start_token_index + get_production_length(child_state),
                                                                      table, tokens);
              break;
            }
          }
        }
        symbol_idx++;
      }
      
      // Handle BLOCK_EXPRESSION_OR_SEMICOLON
      if (symbol_idx < production.size() && std::holds_alternative<Nonterminal>(production[symbol_idx]) &&
          std::get<Nonterminal>(production[symbol_idx]) == Nonterminal::BLOCK_EXPRESSION_OR_SEMICOLON) {
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::BLOCK_EXPRESSION_OR_SEMICOLON) &&
                is_finished_state(child_state, parse_rules)) {
              func_node->block_expression_or_semicolon = construct_cst(child_state, child_state.start_token_index,
                                                                      child_state.start_token_index + get_production_length(child_state),
                                                                      table, tokens);
              break;
            }
          }
        }
      }
      break;
    }
    
    case Nonterminal::OPTIONAL_CONST: {
      auto opt_const_node = static_cast<OptionalConstNode*>(node.get());
      if (production_index == 0) { // "const"
        // Empty - will be set to "const" by the token value
        opt_const_node->value = "const";
      } else {
        // Epsilon - empty string
        opt_const_node->value = "";
      }
      break;
    }
    
    case Nonterminal::STRUCT_FIELDS: {
      auto struct_fields_node = static_cast<StructFieldsNode*>(node.get());
      // STRUCT_FIELD COMMA_STRUCT_FIELDS OPTIONAL_COMMA
      // First child: STRUCT_FIELD
      for (std::size_t r = i; r <= j; ++r) {
        if (r >= table.size()) continue;
        const auto& chart = table[r];
        for (const auto& child_state : chart) {
          if (child_state.nonterminal_type == static_cast<int>(Nonterminal::STRUCT_FIELD) &&
              is_finished_state(child_state, parse_rules)) {
            struct_fields_node->struct_fields.push_back(construct_cst(child_state, child_state.start_token_index,
                                                                     child_state.start_token_index + get_production_length(child_state),
                                                                     table, tokens));
            break;
          }
        }
      }
      // Second child: COMMA_STRUCT_FIELDS
      for (std::size_t r = i; r <= j; ++r) {
        if (r >= table.size()) continue;
        const auto& chart = table[r];
        for (const auto& child_state : chart) {
          if (child_state.nonterminal_type == static_cast<int>(Nonterminal::COMMA_STRUCT_FIELDS) &&
              is_finished_state(child_state, parse_rules)) {
            auto comma_fields_node = static_cast<CommaStructFieldsNode*>(construct_cst(child_state, child_state.start_token_index,
                                                                                       child_state.start_token_index + get_production_length(child_state),
                                                                                       table, tokens).release());
            // Move fields from comma node to struct fields node
            for (auto& field : comma_fields_node->struct_fields) {
              struct_fields_node->struct_fields.push_back(std::move(field));
            }
            delete comma_fields_node;
            break;
          }
        }
      }
      // Third child: OPTIONAL_COMMA
      for (std::size_t r = i; r <= j; ++r) {
        if (r >= table.size()) continue;
        const auto& chart = table[r];
        for (const auto& child_state : chart) {
          if (child_state.nonterminal_type == static_cast<int>(Nonterminal::OPTIONAL_COMMA) &&
              is_finished_state(child_state, parse_rules)) {
            struct_fields_node->optional_comma = construct_cst(child_state, child_state.start_token_index,
                                                              child_state.start_token_index + get_production_length(child_state),
                                                              table, tokens);
            break;
          }
        }
      }
      break;
    }
    
    case Nonterminal::STRUCT_FIELD: {
      auto struct_field_node = static_cast<StructFieldNode*>(node.get());
      // Identifier ":" TYPE
      std::size_t symbol_idx = 0;
      
      // Identifier
      if (symbol_idx < production.size() && std::holds_alternative<Token>(production[symbol_idx])) {
        const Token& identifier_token = std::get<Token>(production[symbol_idx]);
        if (identifier_token.type == Token::Type::Identifier && token_pos < tokens.size()) {
          struct_field_node->identifier = tokens[token_pos].value;
          token_pos++;
        }
        symbol_idx++;
      }
      
      // Skip ":"
      symbol_idx++;
      
      // TYPE
      if (symbol_idx < production.size() && std::holds_alternative<Nonterminal>(production[symbol_idx])) {
        Nonterminal type_nonterminal = std::get<Nonterminal>(production[symbol_idx]);
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(type_nonterminal) &&
                is_finished_state(child_state, parse_rules)) {
              struct_field_node->type = construct_cst(child_state, child_state.start_token_index,
                                                     child_state.start_token_index + get_production_length(child_state),
                                                     table, tokens);
              break;
            }
          }
        }
      }
      break;
    }
    
    case Nonterminal::STATEMENTS: {
      auto statements_node = static_cast<StatementsNode*>(node.get());
      if (production_index == 0) { // STATEMENTS -> STATEMENTS STATEMENT
        // First child: STATEMENTS
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::STATEMENTS) &&
                is_finished_state(child_state, parse_rules)) {
              auto prev_statements_node = static_cast<StatementsNode*>(construct_cst(child_state, child_state.start_token_index,
                                                                                     child_state.start_token_index + get_production_length(child_state),
                                                                                     table, tokens).release());
              // Move statements from previous node
              for (auto& stmt : prev_statements_node->statements) {
                statements_node->statements.push_back(std::move(stmt));
              }
              delete prev_statements_node;
              break;
            }
          }
        }
        // Second child: STATEMENT
        for (std::size_t r = i; r <= j; ++r) {
          if (r >= table.size()) continue;
          const auto& chart = table[r];
          for (const auto& child_state : chart) {
            if (child_state.nonterminal_type == static_cast<int>(Nonterminal::STATEMENT) &&
                is_finished_state(child_state, parse_rules)) {
              statements_node->statements.push_back(construct_cst(child_state, child_state.start_token_index,
                                                                 child_state.start_token_index + get_production_length(child_state),
                                                                 table, tokens));
              break;
            }
          }
        }
      }
      // Epsilon production - empty vector
      break;
    }
    
    default: {
      // For other node types, use the generic approach for simple cases
      // This handles nodes with simple single-child or vector-child structures
      
      // Check if this is a node with a vector field (like items, fields, params, etc.)
      bool is_vector_node = false;
      std::string vector_field_name;
      
      // Determine if this nonterminal typically has vector children
      switch (static_cast<Nonterminal>(state.nonterminal_type)) {
        case Nonterminal::COMMA_STRUCT_FIELDS:
        case Nonterminal::COMMA_FUNCTION_PARAMS:
        case Nonterminal::COMMA_ARRAY_ELEMENTS:
        case Nonterminal::COMMA_CALL_PARAMS:
        case Nonterminal::COMMA_STRUCT_EXPR_FIELDS:
        case Nonterminal::COMMA_ENUM_VARIANTS:
          is_vector_node = true;
          break;
        default:
          break;
      }
      
      if (is_vector_node) {
        // For comma-separated lists, collect all the child elements
        for (const auto& symbol : production) {
          if (std::holds_alternative<Nonterminal>(symbol)) {
            Nonterminal child_nonterminal = std::get<Nonterminal>(symbol);
            for (std::size_t r = i; r <= j; ++r) {
              if (r >= table.size()) continue;
              const auto& chart = table[r];
              for (const auto& child_state : chart) {
                if (child_state.nonterminal_type == static_cast<int>(child_nonterminal) &&
                    is_finished_state(child_state, parse_rules)) {
                  // For vector nodes, we'll handle them in their specific cases above
                  break;
                }
              }
            }
          }
        }
      } else {
        // For single-child nodes, find and assign the child
        for (const auto& symbol : production) {
          if (std::holds_alternative<Nonterminal>(symbol)) {
            Nonterminal child_nonterminal = std::get<Nonterminal>(symbol);
            for (std::size_t r = i; r <= j; ++r) {
              if (r >= table.size()) continue;
              const auto& chart = table[r];
              for (const auto& child_state : chart) {
                if (child_state.nonterminal_type == static_cast<int>(child_nonterminal) &&
                    is_finished_state(child_state, parse_rules)) {
                  // For generic single-child nodes, we can't easily assign to specific fields
                  // This would need to be handled on a case-by-case basis
                  construct_cst(child_state, child_state.start_token_index,
                               child_state.start_token_index + get_production_length(child_state),
                               table, tokens);
                  break;
                }
              }
            }
          } else if (std::holds_alternative<Token>(symbol)) {
            // Handle terminal tokens for nodes that store them as strings
            const Token& token = std::get<Token>(symbol);
            if (token_pos < tokens.size()) {
              // This is a generic handler - specific nodes handle their terminals explicitly above
              token_pos++;
            }
          }
        }
      }
      break;
    }
  }
  
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