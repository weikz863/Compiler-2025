#include "parse_tree.hpp"

// Terminals
std::any IdentifierNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any KeywordNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CharLiteralNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StringLiteralNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any IntegerLiteralNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PunctuationNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any WhitespaceNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommentNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

// Nonterminals
std::any ItemsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ItemNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any FunctionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalConstNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any FunctionParametersNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any SelfParamNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ShorthandSelfNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any TypedSelfNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any FunctionParamNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any FunctionReturnTypeNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalFunctionParametersNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalCommaNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommaFunctionParamsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalFunctionReturnTypeNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any BlockExpressionOrSemicolonNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StructNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StructFieldsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StructFieldNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalStructFieldsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommaStructFieldsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any EnumerationNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any EnumVariantsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any EnumVariantNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalEnumVariantsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommaEnumVariantsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ConstantItemNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any TraitNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ImplementationNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any InherentImplNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any TraitImplNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StatementNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any LetStatementNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ExpressionStatementNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any Unused1Node::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any BasicExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any LiteralExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any UnderscoreExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any GroupedExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ArrayExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalArrayElementsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ArrayElementsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommaArrayElementsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PathExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StructExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalStructExprFieldsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StructExprFieldsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommaStructExprFieldsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StructExprFieldNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PostfixExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any MethodCallExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OptionalCallParamsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CallParamsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CommaCallParamsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any FieldExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CallExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any IndexExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any UnaryOperatorExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any BorrowExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any DereferenceExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any NegationExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any TypeCastExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any MultiplicativeOperatorExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any AdditiveOperatorExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ShiftOperatorExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any AndExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any XorExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any OrExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ComparisonOperatorExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any LazyAndExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any LazyOrExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any AssignmentExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any SimpleAssignmentExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any CompoundAssignmentExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any FlowControlExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ContinueExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any BreakExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ReturnExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ExpressionWithBlockNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any BlockExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any StatementsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any LoopExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any InfiniteLoopExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PredicateLoopExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any IfExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ConditionsNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PatternNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any IdentifierPatternNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any WildcardPatternNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ReferencePatternNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any TypeNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any TypePathNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ReferenceTypeNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any ArrayTypeNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any UnitTypeNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PathInExpressionNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}

std::any PathExprSegmentNode::accept(TreeVisitor& visitor) override {
  return visitor.visit(*this);
}