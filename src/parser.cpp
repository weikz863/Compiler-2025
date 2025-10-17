#include "parser.hpp"
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
  std::cerr << "DEBUG: add_to_set called - Chart:" << chart_index
            << " NT:" << state.nonterminal_type
            << " Prod:" << state.production_index
            << " Pos:" << state.position_in_production
            << " Start:" << state.start_token_index << std::endl;

  auto& chart_set = table[chart_index];
  if (std::find(chart_set.begin(), chart_set.end(), state) == chart_set.end()) {
    std::cerr << "DEBUG: Adding new state to chart " << chart_index << std::endl;
    chart_set.push_back(state);
  } else {
    std::cerr << "DEBUG: State already exists in chart " << chart_index << std::endl;
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
  if (true) { // todo
    auto next = next_element(state);
    const Token& expected_token = std::get<Token>(next);
    const Token& current_token = tokens[chart_index];
    std::cerr << "DEBUG: Scanner - Expected token: type=" << static_cast<int>(expected_token.type)
              << " value='" << expected_token.value << "' Current token: type=" << static_cast<int>(current_token.type)
              << " value='" << current_token.value << "'" << std::endl;

    // Check if the current token matches using Token::match method
    if (expected_token.match(current_token)) {
      std::cerr << "DEBUG: Scanner - Match found!" << std::endl;
      ParsingState new_state = state;
      new_state.position_in_production++;
      add_to_set(new_state, chart_index + 1);
    } else {
      std::cerr << "DEBUG: Scanner - No match" << std::endl;
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

    std::cerr << "DEBUG: Processing chart " << k << " with " << table[k].size() << " states" << std::endl;

    // Process all states in S[k] - states can expand during this loop
    for (std::size_t state_index = 0; state_index < table[k].size(); state_index++) {
      const ParsingState state = *std::next(table[k].begin(), state_index);

        std::cerr << "DEBUG: Processing state - NT:" << state.nonterminal_type
                  << " Prod:" << state.production_index
                  << " Pos:" << state.position_in_production
                  << " Start:" << state.start_token_index << std::endl;

      if (is_finished(state)) {
        std::cerr << "DEBUG: State is finished, calling completer" << std::endl;
        completer(state, k);
      } else if (k < tokens.size()) {
        auto next = next_element(state);
        if (is_nonterminal(next)) {
          std::cerr << "DEBUG: Next element is nonterminal, calling predictor" << std::endl;
          predictor(state, k);
        } else if (is_terminal(next)) {
          std::cerr << "DEBUG: Next element is terminal, calling scanner" << std::endl;
          scanner(state, k);
        }
      }
    }
    std::cerr << "DEBUG: Finished processing chart " << k << ", final size: " << table[k].size() << " states" << std::endl;
  }
  std::cerr << "DEBUG: Parser constructor completed" << std::endl;
}

bool EarleyParser::accepts() const {
  // Check if we have a completed parse in the final chart
  if (table.empty()) return false;
  const auto& final_chart = table.back();
  for (const auto& state : final_chart) {
    // Look for a state that represents a completed ITEMS production
    if (state.nonterminal_type == static_cast<int>(Nonterminal::ITEMS) &&
        state.start_token_index == 0 &&
        is_finished(state)) {
      return true;
    }
  }
  return false;
}