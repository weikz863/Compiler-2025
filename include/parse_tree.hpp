#pragma once

#ifndef _PARSE_TREE_HPP_
#define _PARSE_TREE_HPP_
#include <any>
#include <memory>
#include <vector>

// visitor pattern - forward declarations
class TreeVisitor;

// Base TreeNode class
class TreeNode {
public:
  virtual ~TreeNode() = default;
  virtual std::any accept(TreeVisitor&) = 0;
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

class ItemsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> items;
};

class ItemNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> item;
};

class FunctionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> optional_const;
  std::string identifier;
  std::unique_ptr<TreeNode> optional_function_parameters;
  std::unique_ptr<TreeNode> optional_function_return_type;
  std::unique_ptr<TreeNode> block_expression_or_semicolon;
};

class OptionalConstNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value; // "const" or empty
};

class FunctionParametersNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> self_param;
  std::unique_ptr<TreeNode> optional_comma;
  std::vector<std::unique_ptr<TreeNode>> function_params;
  std::unique_ptr<TreeNode> comma_function_params;
};

class SelfParamNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> shorthand_self;
  std::unique_ptr<TreeNode> typed_self;
};

class ShorthandSelfNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string ampersand; // "&" or empty
  std::string mut; // "mut" or empty
  std::string self; // "self"
};

class TypedSelfNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string mut; // "mut" or empty
  std::string self; // "self"
  std::unique_ptr<TreeNode> type;
};

class FunctionParamNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> pattern;
  std::unique_ptr<TreeNode> type;
};

class FunctionReturnTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> type;
};

class OptionalFunctionParametersNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> function_parameters;
};

class OptionalCommaNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value; // "," or empty
};

class CommaFunctionParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> function_params;
};

class OptionalFunctionReturnTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> function_return_type;
};

class BlockExpressionOrSemicolonNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> block_expression;
  std::string semicolon; // ";" or empty
};

class StructNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> optional_struct_fields;
  std::string semicolon; // ";" or empty
};

class StructFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> struct_fields;
  std::unique_ptr<TreeNode> optional_comma;
};

class StructFieldNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> type;
};

class OptionalStructFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> struct_fields;
};

class CommaStructFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> struct_fields;
};

class EnumerationNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> optional_enum_variants;
};

class EnumVariantsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> enum_variants;
  std::unique_ptr<TreeNode> optional_comma;
};

class EnumVariantNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
};

class OptionalEnumVariantsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> enum_variants;
};

class CommaEnumVariantsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> enum_variants;
};

class ConstantItemNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> type;
  std::unique_ptr<TreeNode> expression; // optional
};

class TraitNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> items;
};

class ImplementationNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> inherent_impl;
  std::unique_ptr<TreeNode> trait_impl;
};

class InherentImplNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> type;
  std::unique_ptr<TreeNode> items;
};

class TraitImplNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> type;
  std::unique_ptr<TreeNode> items;
};

class StatementNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string semicolon; // ";" or empty
  std::unique_ptr<TreeNode> item;
  std::unique_ptr<TreeNode> let_statement;
  std::unique_ptr<TreeNode> expression_statement;
};

class LetStatementNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> pattern;
  std::unique_ptr<TreeNode> type;
  std::unique_ptr<TreeNode> expression; // optional
};

class ExpressionStatementNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> expression;
  std::unique_ptr<TreeNode> expression_with_block;
};

class ExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> flow_control_expression;
};

class Unused1Node : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class BasicExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> literal_expression;
  std::unique_ptr<TreeNode> underscore_expression;
  std::unique_ptr<TreeNode> grouped_expression;
  std::unique_ptr<TreeNode> array_expression;
  std::unique_ptr<TreeNode> path_expression;
  std::unique_ptr<TreeNode> struct_expression;
  std::unique_ptr<TreeNode> expression_with_block;
};

class LiteralExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> char_literal;
  std::unique_ptr<TreeNode> string_literal;
  std::unique_ptr<TreeNode> integer_literal;
  std::string true_keyword; // "true" or empty
  std::string false_keyword; // "false" or empty
};

class UnderscoreExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value; // "_"
};

class GroupedExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> expression;
};

class ArrayExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> optional_array_elements;
};

class OptionalArrayElementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> array_elements;
};

class ArrayElementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> expressions;
  std::unique_ptr<TreeNode> optional_comma;
  std::unique_ptr<TreeNode> expression; // for [expr; expr]
  std::unique_ptr<TreeNode> size_expression;
};

class CommaArrayElementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> expressions;
};

class PathExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> path_in_expression;
};

class StructExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> path_in_expression;
  std::unique_ptr<TreeNode> optional_struct_expr_fields;
};

class OptionalStructExprFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> struct_expr_fields;
};

class StructExprFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> struct_expr_fields;
  std::unique_ptr<TreeNode> optional_comma;
};

class CommaStructExprFieldsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> struct_expr_fields;
};

class StructExprFieldNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::unique_ptr<TreeNode> expression;
};

class PostfixExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> basic_expression;
  std::unique_ptr<TreeNode> method_call_expression;
  std::unique_ptr<TreeNode> field_expression;
  std::unique_ptr<TreeNode> call_expression;
  std::unique_ptr<TreeNode> index_expression;
};

class MethodCallExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> postfix_expression;
  std::unique_ptr<TreeNode> path_expr_segment;
  std::unique_ptr<TreeNode> optional_call_params;
};

class OptionalCallParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> call_params;
};

class CallParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> expressions;
  std::unique_ptr<TreeNode> optional_comma;
};

class CommaCallParamsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> expressions;
};

class FieldExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> postfix_expression;
  std::string identifier;
};

class CallExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> postfix_expression;
  std::unique_ptr<TreeNode> optional_call_params;
};

class IndexExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> postfix_expression;
  std::unique_ptr<TreeNode> expression;
};

class UnaryOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> postfix_expression;
  std::unique_ptr<TreeNode> borrow_expression;
  std::unique_ptr<TreeNode> dereference_expression;
  std::unique_ptr<TreeNode> negation_expression;
};

class BorrowExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string ampersand; // "&" or "&&"
  std::string mut; // "mut" or empty
  std::unique_ptr<TreeNode> unary_operator_expression;
};

class DereferenceExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> unary_operator_expression;
};

class NegationExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string operator_; // "!" or "-"
  std::unique_ptr<TreeNode> unary_operator_expression;
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
  std::unique_ptr<TreeNode> type_cast_expression;
  std::string operator_; // "*" or "/" or "%"
  std::unique_ptr<TreeNode> multiplicative_operator_expression;
};

class AdditiveOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> multiplicative_operator_expression;
  std::string operator_; // "+" or "-"
  std::unique_ptr<TreeNode> additive_operator_expression;
};

class ShiftOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> additive_operator_expression;
  std::string operator_; // "<<" or ">>"
  std::unique_ptr<TreeNode> shift_operator_expression;
};

class AndExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> shift_operator_expression;
  std::unique_ptr<TreeNode> and_expression;
};

class XorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> and_expression;
  std::unique_ptr<TreeNode> xor_expression;
};

class OrExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> xor_expression;
  std::unique_ptr<TreeNode> or_expression;
};

class ComparisonOperatorExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> or_expression;
  std::string operator_; // "==" or "!=" or "<" or "<=" or ">" or ">="
  std::unique_ptr<TreeNode> or_expression_right;
};

class LazyAndExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> comparison_operator_expression;
  std::unique_ptr<TreeNode> lazy_and_expression;
};

class LazyOrExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> lazy_and_expression;
  std::unique_ptr<TreeNode> lazy_or_expression;
};

class AssignmentExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> lazy_or_expression;
  std::unique_ptr<TreeNode> simple_assignment_expression;
  std::unique_ptr<TreeNode> compound_assignment_expression;
};

class SimpleAssignmentExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> lazy_or_expression;
  std::unique_ptr<TreeNode> assignment_expression;
};

class CompoundAssignmentExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> lazy_or_expression;
  std::string operator_; // "+=" etc.
  std::unique_ptr<TreeNode> assignment_expression;
};

class FlowControlExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> assignment_expression;
  std::unique_ptr<TreeNode> continue_expression;
  std::unique_ptr<TreeNode> break_expression;
  std::unique_ptr<TreeNode> return_expression;
};

class ContinueExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value; // "continue"
};

class BreakExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> flow_control_expression; // optional
};

class ReturnExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> flow_control_expression; // optional
};

class ExpressionWithBlockNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> block_expression;
  std::unique_ptr<TreeNode> loop_expression;
  std::unique_ptr<TreeNode> if_expression;
};

class BlockExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> statements;
  std::unique_ptr<TreeNode> expression; // optional
};

class StatementsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::vector<std::unique_ptr<TreeNode>> statements;
};

class LoopExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> infinite_loop_expression;
  std::unique_ptr<TreeNode> predicate_loop_expression;
};

class InfiniteLoopExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> block_expression;
};

class PredicateLoopExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> conditions;
  std::unique_ptr<TreeNode> block_expression;
};

class IfExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> conditions;
  std::unique_ptr<TreeNode> block_expression;
  std::unique_ptr<TreeNode> else_if_expression; // optional
  std::unique_ptr<TreeNode> else_block_expression; // optional
};

class ConditionsNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> expression;
};

class PatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> identifier_pattern;
  std::unique_ptr<TreeNode> wildcard_pattern;
  std::unique_ptr<TreeNode> reference_pattern;
};

class IdentifierPatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string ref; // "ref" or empty
  std::string mut; // "mut" or empty
  std::string identifier;
};

class WildcardPatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string value; // "_"
};

class ReferencePatternNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string ampersand; // "&" or "&&"
  std::string mut; // "mut" or empty
  std::unique_ptr<TreeNode> pattern;
};

class TypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> type_path;
  std::unique_ptr<TreeNode> reference_type;
  std::unique_ptr<TreeNode> array_type;
  std::unique_ptr<TreeNode> unit_type;
};

class TypePathNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> path_expr_segment;
};

class ReferenceTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string mut; // "mut" or empty
  std::unique_ptr<TreeNode> type;
};

class ArrayTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> type;
  std::unique_ptr<TreeNode> expression;
};

class UnitTypeNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
};

class PathInExpressionNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::unique_ptr<TreeNode> path_expr_segment;
  std::unique_ptr<TreeNode> path_expr_segment2; // optional
};

class PathExprSegmentNode : public TreeNode {
public:
  std::any accept(TreeVisitor& visitor) override { return visitor.visit(*this); }
  std::string identifier;
  std::string self_keyword; // "Self" or "self" or empty
};

#endif