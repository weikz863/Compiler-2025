#ifndef _PARSE_TREE_HPP_
#define _PARSE_TREE_HPP_

#include <memory>
#include "parser.hpp"

class TreeNode;

class VisitorBase {
 public:
  virtual ~VisitorBase();
  virtual void visit(TreeNode &) = 0;
};

class TreeNode {
 public:
  TreeNode(const EarleyParser &, std::size_t, std::size_t);
  TreeNode(const std::string &);

  void accept(VisitorBase &);

  std::variant<ParsingState, std::string> data;
  std::vector<std::unique_ptr<TreeNode>> children;
};

#endif