# description of the contents of parse_tree.hpp and parse_tree.cpp

`parse_tree.hpp` contains the class definition for the `TreeNode` and `TreeVisitor` base classes.

It also contains class definitions for each kind of `TreeNode` that can be created by the parser.

The `TreeNode` classes may have child node pointers or data members, according to their kind. Each kind of terminal (token) and nonterminal in the parsing process corresponds to one kind of `TreeNode`, that is, one class that inherits from `TreeNode`.

Detailed member descriptions: see the parse rules in parse_rules.cpp. For every kind of `TreeNode` that is a nonterminal, each of its production rule introduces all of its components as child node pointers. For example, in `parse_rules.cpp`, we have:

```
  // 87. IDENTIFIER_PATTERN -> "ref"? "mut"? Identifier
  {
    { Token(Token::Type::Keyword, "ref"), Token(Token::Type::Keyword, "mut"), 
      Token(Token::Type::Identifier) },
    { Token(Token::Type::Keyword, "ref"), Token(Token::Type::Identifier) },
    { Token(Token::Type::Keyword, "mut"), Token(Token::Type::Identifier) },
    { Token(Token::Type::Identifier) }
  },
```

`IdentifierPatternNode` has two pointers to `KeywordNode` indicating `ref` and `mut`. If there is no `ref` or `mut` found in actual parsing, the pointers are left empty. It also has a pointer to `IdentifierNode`. This pointer must not be empty, according to the production rules.

Terminal token nodes such as `KeywordNode` hava no pointer data members, only a string holding its value, for example `ref` and `mut`.

Each `TreeNode` class has a `accept()` method, per the visitor pattern.

It also contains the class definition for the `DebugTreeVisitor` class, which is a visitor that prints the tree structure.

`parse_tree.hpp` contains member functions for the `DebugTreeVisitor` class.
