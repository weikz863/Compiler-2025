#pragma once

#ifndef _PARSE_TREE_HPP_
#define _PARSE_TREE_HPP_
#include <any>
#include <iostream>
#include <memory>
#include <vector>

// visitor pattern - forward declarations
class TreeVisitor;

// Base TreeNode class
class TreeNode {
public:
   virtual ~TreeNode() = default;
   virtual std::any accept(TreeVisitor&) = 0;
   std::vector<std::unique_ptr<TreeNode>> children;
};

// Forward declarations for all node types
class IdentifierNode;
class KeywordNode;
class CharLiteralNode;
class StringLiteralNode;
class IntegerLiteralNode;
class PunctuationNode;
class WhitespaceNode;
class CommentNode;
class ItemsNode;
class ItemNode;
class FunctionNode;
class OptionalConstNode;
class FunctionParametersNode;
class SelfParamNode;
class ShorthandSelfNode;
class TypedSelfNode;
class FunctionParamNode;
class FunctionReturnTypeNode;
class OptionalFunctionParametersNode;
class OptionalCommaNode;
class CommaFunctionParamsNode;
class OptionalFunctionReturnTypeNode;
class BlockExpressionOrSemicolonNode;
class StructNode;
class StructFieldsNode;
class StructFieldNode;
class OptionalStructFieldsNode;
class CommaStructFieldsNode;
class EnumerationNode;
class EnumVariantsNode;
class EnumVariantNode;
class OptionalEnumVariantsNode;
class CommaEnumVariantsNode;
class ConstantItemNode;
class TraitNode;
class ImplementationNode;
class InherentImplNode;
class TraitImplNode;
class StatementNode;
class LetStatementNode;
class ExpressionStatementNode;
class ExpressionNode;
class Unused1Node;
class BasicExpressionNode;
class LiteralExpressionNode;
class UnderscoreExpressionNode;
class GroupedExpressionNode;
class ArrayExpressionNode;
class OptionalArrayElementsNode;
class ArrayElementsNode;
class CommaArrayElementsNode;
class PathExpressionNode;
class StructExpressionNode;
class OptionalStructExprFieldsNode;
class StructExprFieldsNode;
class CommaStructExprFieldsNode;
class StructExprFieldNode;
class PostfixExpressionNode;
class MethodCallExpressionNode;
class OptionalCallParamsNode;
class CallParamsNode;
class CommaCallParamsNode;
class FieldExpressionNode;
class CallExpressionNode;
class IndexExpressionNode;
class UnaryOperatorExpressionNode;
class BorrowExpressionNode;
class DereferenceExpressionNode;
class NegationExpressionNode;
class TypeCastExpressionNode;
class MultiplicativeOperatorExpressionNode;
class AdditiveOperatorExpressionNode;
class ShiftOperatorExpressionNode;
class AndExpressionNode;
class XorExpressionNode;
class OrExpressionNode;
class ComparisonOperatorExpressionNode;
class LazyAndExpressionNode;
class LazyOrExpressionNode;
class AssignmentExpressionNode;
class SimpleAssignmentExpressionNode;
class CompoundAssignmentExpressionNode;
class FlowControlExpressionNode;
class ContinueExpressionNode;
class BreakExpressionNode;
class ReturnExpressionNode;
class ExpressionWithBlockNode;
class BlockExpressionNode;
class StatementsNode;
class LoopExpressionNode;
class InfiniteLoopExpressionNode;
class PredicateLoopExpressionNode;
class IfExpressionNode;
class ConditionsNode;
class PatternNode;
class IdentifierPatternNode;
class WildcardPatternNode;
class ReferencePatternNode;
class TypeNode;
class TypePathNode;
class ReferenceTypeNode;
class ArrayTypeNode;
class UnitTypeNode;
class PathInExpressionNode;
class PathExprSegmentNode;

class TreeVisitor {
public:
  virtual ~TreeVisitor() = default;
  // Terminals
  virtual std::any visit(IdentifierNode&) = 0;
  virtual std::any visit(KeywordNode&) = 0;
  virtual std::any visit(CharLiteralNode&) = 0;
  virtual std::any visit(StringLiteralNode&) = 0;
  virtual std::any visit(IntegerLiteralNode&) = 0;
  virtual std::any visit(PunctuationNode&) = 0;
  virtual std::any visit(WhitespaceNode&) = 0;
  virtual std::any visit(CommentNode&) = 0;
  // Nonterminals
  virtual std::any visit(ItemsNode&) = 0;
  virtual std::any visit(ItemNode&) = 0;
  virtual std::any visit(FunctionNode&) = 0;
  virtual std::any visit(OptionalConstNode&) = 0;
  virtual std::any visit(FunctionParametersNode&) = 0;
  virtual std::any visit(SelfParamNode&) = 0;
  virtual std::any visit(ShorthandSelfNode&) = 0;
  virtual std::any visit(TypedSelfNode&) = 0;
  virtual std::any visit(FunctionParamNode&) = 0;
  virtual std::any visit(FunctionReturnTypeNode&) = 0;
  virtual std::any visit(OptionalFunctionParametersNode&) = 0;
  virtual std::any visit(OptionalCommaNode&) = 0;
  virtual std::any visit(CommaFunctionParamsNode&) = 0;
  virtual std::any visit(OptionalFunctionReturnTypeNode&) = 0;
  virtual std::any visit(BlockExpressionOrSemicolonNode&) = 0;
  virtual std::any visit(StructNode&) = 0;
  virtual std::any visit(StructFieldsNode&) = 0;
  virtual std::any visit(StructFieldNode&) = 0;
  virtual std::any visit(OptionalStructFieldsNode&) = 0;
  virtual std::any visit(CommaStructFieldsNode&) = 0;
  virtual std::any visit(EnumerationNode&) = 0;
  virtual std::any visit(EnumVariantsNode&) = 0;
  virtual std::any visit(EnumVariantNode&) = 0;
  virtual std::any visit(OptionalEnumVariantsNode&) = 0;
  virtual std::any visit(CommaEnumVariantsNode&) = 0;
  virtual std::any visit(ConstantItemNode&) = 0;
  virtual std::any visit(TraitNode&) = 0;
  virtual std::any visit(ImplementationNode&) = 0;
  virtual std::any visit(InherentImplNode&) = 0;
  virtual std::any visit(TraitImplNode&) = 0;
  virtual std::any visit(StatementNode&) = 0;
  virtual std::any visit(LetStatementNode&) = 0;
  virtual std::any visit(ExpressionStatementNode&) = 0;
  virtual std::any visit(ExpressionNode&) = 0;
  virtual std::any visit(Unused1Node&) = 0;
  virtual std::any visit(BasicExpressionNode&) = 0;
  virtual std::any visit(LiteralExpressionNode&) = 0;
  virtual std::any visit(UnderscoreExpressionNode&) = 0;
  virtual std::any visit(GroupedExpressionNode&) = 0;
  virtual std::any visit(ArrayExpressionNode&) = 0;
  virtual std::any visit(OptionalArrayElementsNode&) = 0;
  virtual std::any visit(ArrayElementsNode&) = 0;
  virtual std::any visit(CommaArrayElementsNode&) = 0;
  virtual std::any visit(PathExpressionNode&) = 0;
  virtual std::any visit(StructExpressionNode&) = 0;
  virtual std::any visit(OptionalStructExprFieldsNode&) = 0;
  virtual std::any visit(StructExprFieldsNode&) = 0;
  virtual std::any visit(CommaStructExprFieldsNode&) = 0;
  virtual std::any visit(StructExprFieldNode&) = 0;
  virtual std::any visit(PostfixExpressionNode&) = 0;
  virtual std::any visit(MethodCallExpressionNode&) = 0;
  virtual std::any visit(OptionalCallParamsNode&) = 0;
  virtual std::any visit(CallParamsNode&) = 0;
  virtual std::any visit(CommaCallParamsNode&) = 0;
  virtual std::any visit(FieldExpressionNode&) = 0;
  virtual std::any visit(CallExpressionNode&) = 0;
  virtual std::any visit(IndexExpressionNode&) = 0;
  virtual std::any visit(UnaryOperatorExpressionNode&) = 0;
  virtual std::any visit(BorrowExpressionNode&) = 0;
  virtual std::any visit(DereferenceExpressionNode&) = 0;
  virtual std::any visit(NegationExpressionNode&) = 0;
  virtual std::any visit(TypeCastExpressionNode&) = 0;
  virtual std::any visit(MultiplicativeOperatorExpressionNode&) = 0;
  virtual std::any visit(AdditiveOperatorExpressionNode&) = 0;
  virtual std::any visit(ShiftOperatorExpressionNode&) = 0;
  virtual std::any visit(AndExpressionNode&) = 0;
  virtual std::any visit(XorExpressionNode&) = 0;
  virtual std::any visit(OrExpressionNode&) = 0;
  virtual std::any visit(ComparisonOperatorExpressionNode&) = 0;
  virtual std::any visit(LazyAndExpressionNode&) = 0;
  virtual std::any visit(LazyOrExpressionNode&) = 0;
  virtual std::any visit(AssignmentExpressionNode&) = 0;
  virtual std::any visit(SimpleAssignmentExpressionNode&) = 0;
  virtual std::any visit(CompoundAssignmentExpressionNode&) = 0;
  virtual std::any visit(FlowControlExpressionNode&) = 0;
  virtual std::any visit(ContinueExpressionNode&) = 0;
  virtual std::any visit(BreakExpressionNode&) = 0;
  virtual std::any visit(ReturnExpressionNode&) = 0;
  virtual std::any visit(ExpressionWithBlockNode&) = 0;
  virtual std::any visit(BlockExpressionNode&) = 0;
  virtual std::any visit(StatementsNode&) = 0;
  virtual std::any visit(LoopExpressionNode&) = 0;
  virtual std::any visit(InfiniteLoopExpressionNode&) = 0;
  virtual std::any visit(PredicateLoopExpressionNode&) = 0;
  virtual std::any visit(IfExpressionNode&) = 0;
  virtual std::any visit(ConditionsNode&) = 0;
  virtual std::any visit(PatternNode&) = 0;
  virtual std::any visit(IdentifierPatternNode&) = 0;
  virtual std::any visit(WildcardPatternNode&) = 0;
  virtual std::any visit(ReferencePatternNode&) = 0;
  virtual std::any visit(TypeNode&) = 0;
  virtual std::any visit(TypePathNode&) = 0;
  virtual std::any visit(ReferenceTypeNode&) = 0;
  virtual std::any visit(ArrayTypeNode&) = 0;
  virtual std::any visit(UnitTypeNode&) = 0;
  virtual std::any visit(PathInExpressionNode&) = 0;
  virtual std::any visit(PathExprSegmentNode&) = 0;
};

// Debug visitor that prints the tree structure
class DebugTreeVisitor : public TreeVisitor {
private:
  int indent_level = 0;
  std::ostream& out;
  const int max_depth = 100;

  void print_indent() const;
  void print_node_start(const std::string& node_type);
  void print_node_with_value(const std::string& node_type, const std::string& value);
  void print_node_end();
  std::any visit_child(const std::unique_ptr<TreeNode>& child);
  void visit_children(const std::vector<std::unique_ptr<TreeNode>>& children);

public:
  DebugTreeVisitor(std::ostream& o = std::cout) : indent_level(0), out(o) {}
  // Terminals
  std::any visit(IdentifierNode& node) override;
  std::any visit(KeywordNode& node) override;
  std::any visit(CharLiteralNode& node) override;
  std::any visit(StringLiteralNode& node) override;
  std::any visit(IntegerLiteralNode& node) override;
  std::any visit(PunctuationNode& node) override;
  std::any visit(WhitespaceNode& node) override;
  std::any visit(CommentNode& node) override;
  
  // Nonterminals
  std::any visit(ItemsNode& node) override;
  std::any visit(ItemNode& node) override;
  std::any visit(FunctionNode& node) override;
  std::any visit(OptionalConstNode& node) override;
  std::any visit(FunctionParametersNode& node) override;
  std::any visit(SelfParamNode& node) override;
  std::any visit(ShorthandSelfNode& node) override;
  std::any visit(TypedSelfNode& node) override;
  std::any visit(FunctionParamNode& node) override;
  std::any visit(FunctionReturnTypeNode& node) override;
  std::any visit(OptionalFunctionParametersNode& node) override;
  std::any visit(OptionalCommaNode& node) override;
  std::any visit(CommaFunctionParamsNode& node) override;
  std::any visit(OptionalFunctionReturnTypeNode& node) override;
  std::any visit(BlockExpressionOrSemicolonNode& node) override;
  std::any visit(StructNode& node) override;
  std::any visit(StructFieldsNode& node) override;
  std::any visit(StructFieldNode& node) override;
  std::any visit(OptionalStructFieldsNode& node) override;
  std::any visit(CommaStructFieldsNode& node) override;
  std::any visit(EnumerationNode& node) override;
  std::any visit(EnumVariantsNode& node) override;
  std::any visit(EnumVariantNode& node) override;
  std::any visit(OptionalEnumVariantsNode& node) override;
  std::any visit(CommaEnumVariantsNode& node) override;
  std::any visit(ConstantItemNode& node) override;
  std::any visit(TraitNode& node) override;
  std::any visit(ImplementationNode& node) override;
  std::any visit(InherentImplNode& node) override;
  std::any visit(TraitImplNode& node) override;
  std::any visit(StatementNode& node) override;
  std::any visit(LetStatementNode& node) override;
  std::any visit(ExpressionStatementNode& node) override;
  std::any visit(ExpressionNode& node) override;
  std::any visit(Unused1Node& node) override;
  std::any visit(BasicExpressionNode& node) override;
  std::any visit(LiteralExpressionNode& node) override;
  std::any visit(UnderscoreExpressionNode& node) override;
  std::any visit(GroupedExpressionNode& node) override;
  std::any visit(ArrayExpressionNode& node) override;
  std::any visit(OptionalArrayElementsNode& node) override;
  std::any visit(ArrayElementsNode& node) override;
  std::any visit(CommaArrayElementsNode& node) override;
  std::any visit(PathExpressionNode& node) override;
  std::any visit(StructExpressionNode& node) override;
  std::any visit(OptionalStructExprFieldsNode& node) override;
  std::any visit(StructExprFieldsNode& node) override;
  std::any visit(CommaStructExprFieldsNode& node) override;
  std::any visit(StructExprFieldNode& node) override;
  std::any visit(PostfixExpressionNode& node) override;
  std::any visit(MethodCallExpressionNode& node) override;
  std::any visit(OptionalCallParamsNode& node) override;
  std::any visit(CallParamsNode& node) override;
  std::any visit(CommaCallParamsNode& node) override;
  std::any visit(FieldExpressionNode& node) override;
  std::any visit(CallExpressionNode& node) override;
  std::any visit(IndexExpressionNode& node) override;
  std::any visit(UnaryOperatorExpressionNode& node) override;
  std::any visit(BorrowExpressionNode& node) override;
  std::any visit(DereferenceExpressionNode& node) override;
  std::any visit(NegationExpressionNode& node) override;
  std::any visit(TypeCastExpressionNode& node) override;
  std::any visit(MultiplicativeOperatorExpressionNode& node) override;
  std::any visit(AdditiveOperatorExpressionNode& node) override;
  std::any visit(ShiftOperatorExpressionNode& node) override;
  std::any visit(AndExpressionNode& node) override;
  std::any visit(XorExpressionNode& node) override;
  std::any visit(OrExpressionNode& node) override;
  std::any visit(ComparisonOperatorExpressionNode& node) override;
  std::any visit(LazyAndExpressionNode& node) override;
  std::any visit(LazyOrExpressionNode& node) override;
  std::any visit(AssignmentExpressionNode& node) override;
  std::any visit(SimpleAssignmentExpressionNode& node) override;
  std::any visit(CompoundAssignmentExpressionNode& node) override;
  std::any visit(FlowControlExpressionNode& node) override;
  std::any visit(ContinueExpressionNode& node) override;
  std::any visit(BreakExpressionNode& node) override;
  std::any visit(ReturnExpressionNode& node) override;
  std::any visit(ExpressionWithBlockNode& node) override;
  std::any visit(BlockExpressionNode& node) override;
  std::any visit(StatementsNode& node) override;
  std::any visit(LoopExpressionNode& node) override;
  std::any visit(InfiniteLoopExpressionNode& node) override;
  std::any visit(PredicateLoopExpressionNode& node) override;
  std::any visit(IfExpressionNode& node) override;
  std::any visit(ConditionsNode& node) override;
  std::any visit(PatternNode& node) override;
  std::any visit(IdentifierPatternNode& node) override;
  std::any visit(WildcardPatternNode& node) override;
  std::any visit(ReferencePatternNode& node) override;
  std::any visit(TypeNode& node) override;
  std::any visit(TypePathNode& node) override;
  std::any visit(ReferenceTypeNode& node) override;
  std::any visit(ArrayTypeNode& node) override;
  std::any visit(UnitTypeNode& node) override;
  std::any visit(PathInExpressionNode& node) override;
  std::any visit(PathExprSegmentNode& node) override;
};

// Terminals
class IdentifierNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class KeywordNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class CharLiteralNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class StringLiteralNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class IntegerLiteralNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class PunctuationNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class WhitespaceNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

class CommentNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value;
};

// Note: For nonterminals, child node pointers are used instead of std::vector/std::string
// The children vector is populated from the production rule components
class ItemsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ItemsNode> items;
  std::unique_ptr<ItemNode> item;
};

class ItemNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> item;
}

class FunctionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<OptionalConstNode> optional_const;
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<OptionalFunctionParametersNode> optional_function_parameters;
  std::unique_ptr<OptionalFunctionReturnTypeNode> optional_function_return_type;
  std::unique_ptr<BlockExpressionOrSemicolonNode> block_expression_or_semicolon;
};

class OptionalConstNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<KeywordNode> keyword; // "const" or nullptr
};

class FunctionParametersNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<SelfParamNode> self_param; // for Rule 0 and Rule 2
  std::unique_ptr<FunctionParamNode> function_param; // for Rule 1
  std::unique_ptr<CommaFunctionParamsNode> comma_function_params; // for Rule 1 and Rule 2
};

class SelfParamNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> self;
};

class ShorthandSelfNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<KeywordNode> ampersand; // "&" or nullptr
  std::unique_ptr<KeywordNode> mut; // "mut" or nullptr
  std::unique_ptr<KeywordNode> self; // "self" (always present)
};

class TypedSelfNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<KeywordNode> mut; // "mut" or nullptr
  std::unique_ptr<KeywordNode> self; // "self" (always present)
  std::unique_ptr<TypeNode> type;
};

class FunctionParamNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PatternNode> pattern;
  std::unique_ptr<TypeNode> type;
};

class FunctionReturnTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TypeNode> type;
};

class OptionalFunctionParametersNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<FunctionParametersNode> function_parameters; // may be nullptr
};

class OptionalCommaNode : public TreeNode { // maybe unused
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PunctuationNode> comma; // "," or nullptr
};

class CommaFunctionParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CommaFunctionParamsNode> comma_function_params; // recursive
  std::unique_ptr<FunctionParamNode> function_param; // for non-empty case
};

class OptionalFunctionReturnTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<FunctionReturnTypeNode> function_return_type; // may be nullptr
};

class BlockExpressionOrSemicolonNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<BlockExpressionNode> block_expression; // may be nullptr indicating ";"
};

class StructNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<OptionalStructFieldsNode> optional_struct_fields; // may be nullptr indicating ";"
};

class StructFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<StructFieldNode> struct_field;
  std::unique_ptr<CommaStructFieldsNode> comma_struct_fields;
};

class StructFieldNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<TypeNode> type;
};

class OptionalStructFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<StructFieldsNode> struct_fields; // may be nullptr
};

class CommaStructFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CommaStructFieldsNode> comma_struct_fields; // recursive
  std::unique_ptr<StructFieldNode> struct_field; // may be nullptr
};

class EnumerationNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<OptionalEnumVariantsNode> optional_enum_variants;
};

class EnumVariantsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<EnumVariantNode> enum_variant;
  std::unique_ptr<CommaEnumVariantsNode> comma_enum_variants;
};

class EnumVariantNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
};

class OptionalEnumVariantsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<EnumVariantsNode> enum_variants; // may be nullptr
};

class CommaEnumVariantsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CommaEnumVariantsNode> comma_enum_variants; // recursive
  std::unique_ptr<EnumVariantNode> enum_variant; // for non-empty case
};

class ConstantItemNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExpressionNode> expression; // may be nullptr
};

class TraitNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<ItemsNode> items;
};

class ImplementationNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> implementation; // InherentImplNode or TraitImplNode
};

class InherentImplNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ItemsNode> items;
};

class TraitImplNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ItemsNode> items;
};

class StatementNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> statement; // polymorphism
};

class LetStatementNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PatternNode> pattern;
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExpressionNode> expression; // may be nullptr
};

class ExpressionStatementNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> expression_statement; // polymorphism
};

class ExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<FlowControlExpressionNode> flow_control_expression;
};

class Unused1Node : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class BasicExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> basic_expression; // ploymorphism
};

class LiteralExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<LiteralExpressionNode> literal_expression; // polymorphism
};

class UnderscoreExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class GroupedExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ExpressionNode> expression;
};

class ArrayExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<OptionalArrayElementsNode> optional_array_elements;
};

class OptionalArrayElementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ArrayElementsNode> array_elements; // may be nullptr
};

class ArrayElementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ExpressionNode> first_expression;
  std::unique_ptr<TreeNode> other; // polymorphism: CommaArrayElementsNode or ExpressionNode
};

class CommaArrayElementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CommaArrayElementsNode> comma_array_elements; // recursive
  std::unique_ptr<ExpressionNode> expression; // maybe nullptr
};

class PathExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PathInExpressionNode> path_in_expression;
};

class StructExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PathInExpressionNode> path_in_expression;
  std::unique_ptr<OptionalStructExprFieldsNode> optional_struct_expr_fields;
};

class OptionalStructExprFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> struct_expr_fields; // may be nullptr
};

class StructExprFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<StructExprFieldsNode> struct_expr_field;
  std::unique_ptr<CommaStructExprFieldsNode> comma_struct_expr_fields;
};

class CommaStructExprFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CommaStructExprFieldsNode> comma_struct_expr_fields; // recursive
  std::unique_ptr<StructExprFieldNode> struct_expr_field; // for non-empty case
};

class StructExprFieldNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<IdentifierNode> identifier;
  std::unique_ptr<ExpressionNode> expression;
};

class PostfixExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> postfix_expression; // polymorphism
};

class MethodCallExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PostfixExpressionNode> postfix_expression;
  std::unique_ptr<PathExprSegmentNode> path_expr_segment;
  std::unique_ptr<OptionalCallParamsNode> optional_call_params;
};

class OptionalCallParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CallParamsNode> call_params; // may be null[tr
};

class CallParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ExpressionNode> expression;
  std::unique_ptr<CommaCallParamsNode> comma_call_params;
};

class CommaCallParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<CommaCallParamsNode> comma_call_params; // recursive
  std::unique_ptr<ExpressionNode> expression; // for non-empty case
};

class FieldExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PosfixExpressionNode> postfix_expression;
  std::unique_ptr<IdentifierNode> identifier;
};

class CallExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PosfixExpressionNode> postfix_expression;
  std::unique_ptr<OptionalCallParamsNode> optional_call_params;
};

class IndexExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PosfixExpressionNode> postfix_expression;
  std::unique_ptr<ExpressionNode> expression;
};

class UnaryOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> unary_operator_expression; // polymorphism
};

class BorrowExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PunctuationNode> ampersands; // "&" or "&&"
  std::unique_ptr<KeywordNode> mut; // "mut" or nullptr
  std::unique_ptr<UnaryOperatorExpressionNode> unary_operator_expression;
};

class DereferenceExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<UnaryOperatorExpressionNode> unary_operator_expression;
};

class NegationExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PunctuationNode> operator_; // "!" or "-"
  std::unique_ptr<UnaryOperatorExpressionNode> unary_operator_expression;
};

class TypeCastExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> unary_operator_expression;
  std::unique_ptr<TreeNode> type_cast_expression;
  std::unique_ptr<TreeNode> type;
};

class MultiplicativeOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<PunctuationNode> operator_; // "*" or "/" or "%"; or nullptr
  std::unique_ptr<TypeCastExpressionNode> second_expression; // may be nullptr
};

class AdditiveOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<PunctuationNode> operator_; // "+" or "-"; or nullptr
  std::unique_ptr<MultiplicativeOperatorExpressionNode> second_expression; // may be nullptr
};

class ShiftOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<PunctuationNode> operator_; // "<<" or ">>"; or nullptr
  std::unique_ptr<AdditiveOperatorExpressionNode> second_expression; // may be nullptr
};

class AndExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<ShiftOperatorExpressionNode> second_expression; // may be nullptr
};

class XorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<AndExpressionNode> second_expression; // may be nullptr
};

class OrExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<XorExpressionNode> second_expression; // may be nullptr
};

class ComparisonOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<OrExpressionNode> first_expression;
  std::unique_ptr<PunctuationNode> operator_; // "==" or "!=" or "<" or "<=" or ">" or ">="; or nullptr
  std::unique_ptr<OrExpressionNode> second_expression; // may be nulptr
};

class LazyAndExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<ComparisonOperatorExpressionNode> second_expression; // may be nullptr
};

class LazyOrExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> first_expression; // polymorphism
  std::unique_ptr<LazyAndExpressionNode> second_expression; // may be nullptr
};

class AssignmentExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> assignment_expression; // polymorphism
};

class SimpleAssignmentExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<LazyOrExpressionNode> left_expression;
  std::unique_ptr<AssignmentExpressionNode> right_expression;
};

class CompoundAssignmentExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<LazyOrExpressionNode> left_expression;
  std::unique_ptr<PunctuationNode> operator_; // "+=" etc.
  std::unique_ptr<AssignmentExpressionNode> right_expression;
};

class FlowControlExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> flow_control_expression; // polymorphism
};

class ContinueExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class BreakExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<FlowControlExpressionNode> flow_control_expression; // may be nullptr
};

class ReturnExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<FlowControlExpressionNode> flow_control_expression; // may be nullptr
};

class ExpressionWithBlockNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> expression_with_block; // polymorphism
};

class BlockExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<StatementsNode> statements;
  std::unique_ptr<ExpressionNode> expression; // may be nullptr
};

class StatementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<StatementsNode> statements; // recursive, may be nullptr
  std::unique_ptr<StatementNode> statement; // may be nullptr
};

class LoopExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> loop_expression; // polymorphism
};

class InfiniteLoopExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<BlockExpressionNode> block_expression;
};

class PredicateLoopExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ConditionsNode> conditions;
  std::unique_ptr<BlockExpressionNode> block_expression;
};

class IfExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ConditionsNode> conditions;
  std::unique_ptr<BlockExpressionNode> block_expression;
  std::unique_ptr<TreeNode> else_expression; // polymorphism, may be nullptr
};

class ConditionsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<ExpressionNode> expression;
};

class PatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> pattern; // polymorphism
};

class IdentifierPatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<KeywordNode> ref; // "ref" or nullptr
  std::unique_ptr<KeywordNode> mut; // "mut" or nullptr
  std::unique_ptr<IdentifierNode> identifier;
};

class WildcardPatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class ReferencePatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PunctuationNode> ampersand; // "&" or "&&"
  std::unique_ptr<KeywordNode> mut; // "mut" or nullptr
  std::unique_ptr<PatternNode> pattern;
};

class TypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> type;
};

class TypePathNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PathExprSegmentNode> path_expr_segment;
};

class ReferenceTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<KeywordNode> mut; // "mut" or nullptr
  std::unique_ptr<TypeNode> type;
};

class ArrayTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TypeNode> type;
  std::unique_ptr<ExpressionNode> expression;
};

class UnitTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class PathInExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<PathExprSegmentNode> path_expr_segment;
  std::unique_ptr<PathExprSegmentNode> path_expr_segment2; // optional
};

class PathExprSegmentNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> path_expr_segment; // polymorphism
};

#endif
