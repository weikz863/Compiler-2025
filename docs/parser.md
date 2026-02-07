# description of the contents of parser.hpp and parser.cpp

`parser.hpp` contains the class definition for the `ParsingState`, `ParseError` and `EarleyParser` classes.

`ParsingState` is a class that represents a state in the Earley parsing algorithm. It contains the following fields:

- `nonterminal_type`: the type of the nonterminal.
- `production_index`: the index of the rule in all productions of the nonterminal.
- `position_in_production`: the position of the dot in the rule.
- `start_token_index`: the index of the token in the input string that the rule starts at.

It also has compsrision operators to show rule precedence.

`ParseError` is a class that represents an error in the parsing process.

`EarleyParser` is a class that represents the Earley parser. The parsing algorithm is implemented in the class constructor.

The `Parse` method is used to generate the parse tree (CST). It reads the parsing table of the Earley parsing method, determines how every terminal and nonterminal symbol in the input string is derived, and constructs the parse tree by creating the appropriate `CSTNode` and linking every terminal and nonterminal used in its derivation to it as a child. It returns a `std::unique_ptr<CSTNode>` object that represents the root of the parse tree. The parse tree contains complete information about the input string, including every terminal and nonterminal symbol in the input string, as well as the production rules used to derive each nonterminal symbol. The information is stored in the `CSTNode` class, which is defined in `parse_tree.hpp`. The information about how each terminal and nonterminal symbol is derived can be recovered completely using the `DebugTreeVisitor`.

The `Parse` method detailed implementation algorithm pseudocode:

```plaintext
// pseudocode that ensures a rightmost derivation and correct rule precedence
// adapted from *A Second Course in Formal Languanges and Automata Theory*

// Find a rightmost derivation of w[i+1 ... j] starting with production A -> β•
PARSE(A -> β•, i, j) 
    If β = X1 X2 ... Xm, set k := m and l := j
    Repeat until k = 0:
        If Xk is terminal:
            set k := k - 1 and l := l - 1
            save node(Xk) as subtree(Xk)
        Else Xk is nonterminal:
            for each production Xk -> γ•, tried in order:
                find maximum possible r such that Xk -> γ• in [r, l]
                  and A -> X1 X2 ... Xk-1 • Xk ... Xm in [i, r] if exists
                if found, break
            Call PARSE(Xk -> γ•, r, l)
            save the returned subtree as subtree(Xk)
        Set k := k - 1 and l := r
    r = new node of type A
    for i = 1 ... m:
        set appropriate child of r to subtree(Xi)
    return r
```

The CST created by the `Parse` method is a tree that represents the input string. It contains all the information about the input string, including every terminal and nonterminal symbol in the input string, as well as the production rules used to derive each nonterminal symbol. Every nonterminal should have a child for every symbol in its production rule.
