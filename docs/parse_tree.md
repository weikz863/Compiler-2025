# description of the contents of parse_tree.hpp and parse_tree.cpp

`parse_tree.hpp` contains the class definition for the `TreeNode` and `TreeVisitor` base classes.

It also contains class definitions for each kind of `TreeNode` that can be created by the parser.
The `TreeNode` classes may have child node pointers or data members, according to their kind.
Detailed member descriptions: see the parse rules in parse_rules.cpp. every production rule introduces
its components as child node pointers if the components are non-terminals, or as data members if the
components are terminals.
Each `TreeNode` class has a `accept()` method, per the visitor pattern.

It also contains the class definition for the `DebugTreeVisitor` class, which is a visitor that prints
the tree structure.

`parse_tree.hpp` contains member functions for the `DebugTreeVisitor` class.
