#include "parse_tree.hpp"

// Debug visitor implementations are provided here
// All accept() methods are implemented inline in the header file

// Debug visitor implementation
void DebugTreeVisitor::print_indent() const {
  for (int i = 0; i < indent_level; ++i) {
    out << "  ";
  }
}

void DebugTreeVisitor::print_node_start(const std::string& node_type) {
  print_indent();
  out << node_type << std::endl;
  indent_level++;
}

void DebugTreeVisitor::print_node_with_value(const std::string& node_type, const std::string& value) {
  print_indent();
  out << node_type << ": \"" << value << "\"" << std::endl;
}

void DebugTreeVisitor::print_node_end() {
  indent_level--;
}

std::any DebugTreeVisitor::visit_child(TreeNode* child) {
  if (child && indent_level < max_depth) {
    return child->accept(*this);
  }
  return std::any();
}

void DebugTreeVisitor::visit_children(const std::vector<std::unique_ptr<TreeNode>>& children) {
  for (const auto& child : children) {
    visit_child(child.get());
  }
}

void DebugTreeVisitor::visit_children(const std::vector<TreeNode*>& children) {
  for (const auto& child : children) {
    visit_child(child);
  }
}

// Terminals
std::any DebugTreeVisitor::visit(IdentifierNode& node) {
  print_node_with_value("IdentifierNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(KeywordNode& node) {
  print_node_with_value("KeywordNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(CharLiteralNode& node) {
  print_node_with_value("CharLiteralNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(StringLiteralNode& node) {
  print_node_with_value("StringLiteralNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(IntegerLiteralNode& node) {
  print_node_with_value("IntegerLiteralNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(PunctuationNode& node) {
  print_node_with_value("PunctuationNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(WhitespaceNode& node) {
  print_node_with_value("WhitespaceNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(CommentNode& node) {
  print_node_with_value("CommentNode", node.value);
  return std::any();
}

// Nonterminals
std::any DebugTreeVisitor::visit(ItemsNode& node) {
  print_node_start("ItemsNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ItemNode& node) {
  print_node_start("ItemNode");
  visit_child(node.item);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionNode& node) {
  print_node_start("FunctionNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalConstNode& node) {
  print_node_start("OptionalConstNode");
  if (node.keyword) {
    visit_child(node.keyword);
  }
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionParametersNode& node) {
  print_node_start("FunctionParametersNode");
  if (node.self_param) visit_child(node.self_param);
  if (node.function_param) visit_child(node.function_param);
  if (node.comma_function_params) visit_child(node.comma_function_params);
  if (node.optional_comma) visit_child(node.optional_comma);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(SelfParamNode& node) {
  print_node_start("SelfParamNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ShorthandSelfNode& node) {
  print_node_start("ShorthandSelfNode");
  if (node.ampersand) visit_child(node.ampersand);
  if (node.mut) visit_child(node.mut);
  if (node.self) visit_child(node.self);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TypedSelfNode& node) {
  print_node_start("TypedSelfNode");
  if (node.mut) visit_child(node.mut);
  if (node.self) visit_child(node.self);
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionParamNode& node) {
  print_node_start("FunctionParamNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionReturnTypeNode& node) {
  print_node_start("FunctionReturnTypeNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalFunctionParametersNode& node) {
  print_node_start("OptionalFunctionParametersNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalCommaNode& node) {
  print_node_start("OptionalCommaNode");
  if (node.comma) visit_child(node.comma);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaFunctionParamsNode& node) {
  print_node_start("CommaFunctionParamsNode");
  if (node.comma_function_params) visit_child(node.comma_function_params);
  if (node.comma) visit_child(node.comma);
  if (node.function_param) visit_child(node.function_param);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalFunctionReturnTypeNode& node) {
  print_node_start("OptionalFunctionReturnTypeNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(BlockExpressionOrSemicolonNode& node) {
  print_node_start("BlockExpressionOrSemicolonNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructNode& node) {
  print_node_start("StructNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.optional_struct_fields.get());
  if (node.semicolon) {
    visit_child(node.semicolon);
  }
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructFieldsNode& node) {
  print_node_start("StructFieldsNode");
  if (node.struct_field) visit_child(node.struct_field);
  if (node.comma_struct_fields) visit_child(node.comma_struct_fields);
  if (node.optional_comma) visit_child(node.optional_comma);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructFieldNode& node) {
  print_node_start("StructFieldNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalStructFieldsNode& node) {
  print_node_start("OptionalStructFieldsNode");
  visit_child(node.struct_fields.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaStructFieldsNode& node) {
  print_node_start("CommaStructFieldsNode");
  if (node.comma_struct_fields) visit_child(node.comma_struct_fields);
  if (node.comma) visit_child(node.comma);
  if (node.struct_field) visit_child(node.struct_field);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(EnumerationNode& node) {
  print_node_start("EnumerationNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.optional_enum_variants.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(EnumVariantsNode& node) {
  print_node_start("EnumVariantsNode");
  if (node.enum_variant) visit_child(node.enum_variant);
  if (node.comma_enum_variants) visit_child(node.comma_enum_variants);
  if (node.optional_comma) visit_child(node.optional_comma);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(EnumVariantNode& node) {
  print_node_start("EnumVariantNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalEnumVariantsNode& node) {
  print_node_start("OptionalEnumVariantsNode");
  visit_child(node.enum_variants.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaEnumVariantsNode& node) {
  print_node_start("CommaEnumVariantsNode");
  if (node.comma_enum_variants) visit_child(node.comma_enum_variants);
  if (node.comma) visit_child(node.comma);
  if (node.enum_variant) visit_child(node.enum_variant);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ConstantItemNode& node) {
  print_node_start("ConstantItemNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.type.get());
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TraitNode& node) {
  print_node_start("TraitNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.items.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ImplementationNode& node) {
  print_node_start("ImplementationNode");
  visit_child(node.inherent_impl.get());
  visit_child(node.trait_impl.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(InherentImplNode& node) {
  print_node_start("InherentImplNode");
  visit_child(node.type.get());
  visit_child(node.items.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TraitImplNode& node) {
  print_node_start("TraitImplNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.type.get());
  visit_child(node.items.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StatementNode& node) {
  print_node_start("StatementNode");
  if (node.semicolon) visit_child(node.semicolon);
  if (node.item) visit_child(node.item.get());
  if (node.let_statement) visit_child(node.let_statement.get());
  if (node.expression_statement) visit_child(node.expression_statement.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(LetStatementNode& node) {
  print_node_start("LetStatementNode");
  visit_child(node.pattern.get());
  visit_child(node.type.get());
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ExpressionStatementNode& node) {
  print_node_start("ExpressionStatementNode");
  visit_child(node.expression.get());
  visit_child(node.expression_with_block.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ExpressionNode& node) {
  print_node_start("ExpressionNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(Unused1Node& node) {
  print_node_start("Unused1Node");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(BasicExpressionNode& node) {
  print_node_start("BasicExpressionNode");
  visit_child(node.literal_expression.get());
  visit_child(node.underscore_expression.get());
  visit_child(node.grouped_expression.get());
  visit_child(node.array_expression.get());
  visit_child(node.path_expression.get());
  visit_child(node.struct_expression.get());
  visit_child(node.expression_with_block.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(LiteralExpressionNode& node) {
  print_node_start("LiteralExpressionNode");
  visit_child(node.char_literal.get());
  visit_child(node.string_literal.get());
  visit_child(node.integer_literal.get());
  if (node.true_keyword) visit_child(node.true_keyword);
  if (node.false_keyword) visit_child(node.false_keyword);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(UnderscoreExpressionNode& node) {
  print_node_with_value("UnderscoreExpressionNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(GroupedExpressionNode& node) {
  print_node_start("GroupedExpressionNode");
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ArrayExpressionNode& node) {
  print_node_start("ArrayExpressionNode");
  visit_child(node.optional_array_elements.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalArrayElementsNode& node) {
  print_node_start("OptionalArrayElementsNode");
  visit_child(node.array_elements.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ArrayElementsNode& node) {
  print_node_start("ArrayElementsNode");
  if (node.expression) visit_child(node.expression);
  if (node.comma_array_elements) visit_child(node.comma_array_elements);
  if (node.optional_comma) visit_child(node.optional_comma);
  if (node.size_expression) visit_child(node.size_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaArrayElementsNode& node) {
  print_node_start("CommaArrayElementsNode");
  if (node.comma_array_elements) visit_child(node.comma_array_elements);
  if (node.comma) visit_child(node.comma);
  if (node.expression) visit_child(node.expression);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(PathExpressionNode& node) {
  print_node_start("PathExpressionNode");
  visit_child(node.path_in_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructExpressionNode& node) {
  print_node_start("StructExpressionNode");
  visit_child(node.path_in_expression.get());
  visit_child(node.optional_struct_expr_fields.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalStructExprFieldsNode& node) {
  print_node_start("OptionalStructExprFieldsNode");
  visit_child(node.struct_expr_fields.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructExprFieldsNode& node) {
  print_node_start("StructExprFieldsNode");
  if (node.struct_expr_field) visit_child(node.struct_expr_field);
  if (node.comma_struct_expr_fields) visit_child(node.comma_struct_expr_fields);
  if (node.optional_comma) visit_child(node.optional_comma);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaStructExprFieldsNode& node) {
  print_node_start("CommaStructExprFieldsNode");
  if (node.comma_struct_expr_fields) visit_child(node.comma_struct_expr_fields);
  if (node.comma) visit_child(node.comma);
  if (node.struct_expr_field) visit_child(node.struct_expr_field);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructExprFieldNode& node) {
  print_node_start("StructExprFieldNode");
  print_indent();
  out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(PostfixExpressionNode& node) {
  print_node_start("PostfixExpressionNode");
  visit_child(node.basic_expression.get());
  visit_child(node.method_call_expression.get());
  visit_child(node.field_expression.get());
  visit_child(node.call_expression.get());
  visit_child(node.index_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(MethodCallExpressionNode& node) {
  print_node_start("MethodCallExpressionNode");
  visit_child(node.postfix_expression.get());
  visit_child(node.path_expr_segment.get());
  visit_child(node.optional_call_params.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalCallParamsNode& node) {
  print_node_start("OptionalCallParamsNode");
  visit_child(node.call_params.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CallParamsNode& node) {
  print_node_start("CallParamsNode");
  if (node.expression) visit_child(node.expression);
  if (node.comma_call_params) visit_child(node.comma_call_params);
  if (node.optional_comma) visit_child(node.optional_comma);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaCallParamsNode& node) {
  print_node_start("CommaCallParamsNode");
  if (node.comma_call_params) visit_child(node.comma_call_params);
  if (node.comma) visit_child(node.comma);
  if (node.expression) visit_child(node.expression);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FieldExpressionNode& node) {
  print_node_start("FieldExpressionNode");
  visit_child(node.postfix_expression.get());
  if (node.identifier) {
    print_indent();
    out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  }
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CallExpressionNode& node) {
  print_node_start("CallExpressionNode");
  visit_child(node.postfix_expression.get());
  visit_child(node.optional_call_params.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(IndexExpressionNode& node) {
  print_node_start("IndexExpressionNode");
  visit_child(node.postfix_expression.get());
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(UnaryOperatorExpressionNode& node) {
  print_node_start("UnaryOperatorExpressionNode");
  visit_child(node.postfix_expression.get());
  visit_child(node.borrow_expression.get());
  visit_child(node.dereference_expression.get());
  visit_child(node.negation_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(BorrowExpressionNode& node) {
  print_node_start("BorrowExpressionNode");
  if (node.ampersand) visit_child(node.ampersand);
  if (node.mut) visit_child(node.mut);
  visit_child(node.unary_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(DereferenceExpressionNode& node) {
  print_node_start("DereferenceExpressionNode");
  visit_child(node.unary_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(NegationExpressionNode& node) {
  print_node_start("NegationExpressionNode");
  if (node.operator_) visit_child(node.operator_);
  visit_child(node.unary_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TypeCastExpressionNode& node) {
  print_node_start("TypeCastExpressionNode");
  visit_child(node.unary_operator_expression.get());
  visit_child(node.type_cast_expression.get());
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(MultiplicativeOperatorExpressionNode& node) {
  print_node_start("MultiplicativeOperatorExpressionNode");
  visit_child(node.type_cast_expression.get());
  if (node.operator_) visit_child(node.operator_);
  visit_child(node.multiplicative_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(AdditiveOperatorExpressionNode& node) {
  print_node_start("AdditiveOperatorExpressionNode");
  visit_child(node.multiplicative_operator_expression.get());
  if (node.operator_) visit_child(node.operator_);
  visit_child(node.additive_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ShiftOperatorExpressionNode& node) {
  print_node_start("ShiftOperatorExpressionNode");
  visit_child(node.additive_operator_expression.get());
  if (node.operator_) visit_child(node.operator_);
  visit_child(node.shift_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(AndExpressionNode& node) {
  print_node_start("AndExpressionNode");
  visit_child(node.shift_operator_expression.get());
  visit_child(node.and_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(XorExpressionNode& node) {
  print_node_start("XorExpressionNode");
  visit_child(node.and_expression.get());
  visit_child(node.xor_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OrExpressionNode& node) {
  print_node_start("OrExpressionNode");
  visit_child(node.xor_expression.get());
  visit_child(node.or_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ComparisonOperatorExpressionNode& node) {
  print_node_start("ComparisonOperatorExpressionNode");
  visit_child(node.or_expression.get());
  if (node.operator_) visit_child(node.operator_);
  visit_child(node.or_expression_right.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(LazyAndExpressionNode& node) {
  print_node_start("LazyAndExpressionNode");
  visit_child(node.comparison_operator_expression.get());
  visit_child(node.lazy_and_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(LazyOrExpressionNode& node) {
  print_node_start("LazyOrExpressionNode");
  visit_child(node.lazy_and_expression.get());
  visit_child(node.lazy_or_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(AssignmentExpressionNode& node) {
  print_node_start("AssignmentExpressionNode");
  visit_child(node.lazy_or_expression.get());
  visit_child(node.simple_assignment_expression.get());
  visit_child(node.compound_assignment_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(SimpleAssignmentExpressionNode& node) {
  print_node_start("SimpleAssignmentExpressionNode");
  visit_child(node.lazy_or_expression.get());
  visit_child(node.assignment_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CompoundAssignmentExpressionNode& node) {
  print_node_start("CompoundAssignmentExpressionNode");
  visit_child(node.lazy_or_expression.get());
  if (node.operator_) visit_child(node.operator_);
  visit_child(node.assignment_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FlowControlExpressionNode& node) {
  print_node_start("FlowControlExpressionNode");
  visit_child(node.assignment_expression.get());
  visit_child(node.continue_expression.get());
  visit_child(node.break_expression.get());
  visit_child(node.return_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ContinueExpressionNode& node) {
  print_node_with_value("ContinueExpressionNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(BreakExpressionNode& node) {
  print_node_start("BreakExpressionNode");
  visit_child(node.flow_control_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ReturnExpressionNode& node) {
  print_node_start("ReturnExpressionNode");
  visit_child(node.flow_control_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ExpressionWithBlockNode& node) {
  print_node_start("ExpressionWithBlockNode");
  visit_child(node.block_expression.get());
  visit_child(node.loop_expression.get());
  visit_child(node.if_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(BlockExpressionNode& node) {
  print_node_start("BlockExpressionNode");
  visit_children(node.children);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StatementsNode& node) {
  print_node_start("StatementsNode");
  if (node.statements) visit_child(node.statements);
  if (node.statement) visit_child(node.statement);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(LoopExpressionNode& node) {
  print_node_start("LoopExpressionNode");
  visit_child(node.infinite_loop_expression.get());
  visit_child(node.predicate_loop_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(InfiniteLoopExpressionNode& node) {
  print_node_start("InfiniteLoopExpressionNode");
  visit_child(node.block_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(PredicateLoopExpressionNode& node) {
  print_node_start("PredicateLoopExpressionNode");
  visit_child(node.conditions.get());
  visit_child(node.block_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(IfExpressionNode& node) {
  print_node_start("IfExpressionNode");
  visit_child(node.conditions.get());
  visit_child(node.block_expression.get());
  visit_child(node.else_if_expression.get());
  visit_child(node.else_block_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ConditionsNode& node) {
  print_node_start("ConditionsNode");
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(PatternNode& node) {
  print_node_start("PatternNode");
  visit_child(node.identifier_pattern.get());
  visit_child(node.wildcard_pattern.get());
  visit_child(node.reference_pattern.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(IdentifierPatternNode& node) {
  print_node_start("IdentifierPatternNode");
  if (node.ref) visit_child(node.ref);
  if (node.mut) visit_child(node.mut);
  if (node.identifier) {
    print_indent();
    out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  }
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(WildcardPatternNode& node) {
  print_node_with_value("WildcardPatternNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(ReferencePatternNode& node) {
  print_node_start("ReferencePatternNode");
  if (node.ampersand) visit_child(node.ampersand);
  if (node.mut) visit_child(node.mut);
  visit_child(node.pattern.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TypeNode& node) {
  print_node_start("TypeNode");
  visit_child(node.type_path.get());
  visit_child(node.reference_type.get());
  visit_child(node.array_type.get());
  visit_child(node.unit_type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TypePathNode& node) {
  print_node_start("TypePathNode");
  visit_child(node.path_expr_segment.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ReferenceTypeNode& node) {
  print_node_start("ReferenceTypeNode");
  if (node.ampersand) visit_child(node.ampersand);
  if (node.mut) visit_child(node.mut);
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ArrayTypeNode& node) {
  print_node_start("ArrayTypeNode");
  visit_child(node.type.get());
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(UnitTypeNode& node) {
  print_node_start("UnitTypeNode");
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(PathInExpressionNode& node) {
  print_node_start("PathInExpressionNode");
  visit_child(node.path_expr_segment.get());
  visit_child(node.path_expr_segment2.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(PathExprSegmentNode& node) {
  print_node_start("PathExprSegmentNode");
  if (node.identifier) {
    print_indent();
    out << "identifier: \"" << node.identifier->value << "\"" << std::endl;
  }
  if (node.self_keyword) visit_child(node.self_keyword);
  print_node_end();
  return std::any();
}

// accept() method implementations for all TreeNode derived classes

// Terminals
std::any IdentifierNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any KeywordNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any CharLiteralNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any StringLiteralNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any IntegerLiteralNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any PunctuationNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any WhitespaceNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any CommentNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

// Nonterminals
std::any ItemsNode::accept(TreeVisitor& visitor) {
  if (this->items) {
    this->items->accept(visitor);
    this->item->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ItemNode::accept(TreeVisitor& visitor) {
  this->item->accept(visitor);
  return visitor.visit(*this);
}

std::any FunctionNode::accept(TreeVisitor& visitor) {
  this->optional_const->accept(visitor);
  this->identifier->accept(visitor);
  this->optional_function_parameters->accept(visitor);
  this->optional_function_return_type->accept(visitor);
  this->block_expression_or_semicolon->accept(visitor);
  return visitor.visit(*this);
}

std::any OptionalConstNode::accept(TreeVisitor& visitor) {
  if (this->keyword) {
    this->keyword->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any FunctionParametersNode::accept(TreeVisitor& visitor) {
  if (this->self_param) {
    this->self_param->accept(visitor);
  }
  if (this->function_param) {
    this->function_param->accept(visitor);
  }
  if (this->comma_function_params) {
    this->comma_function_params->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any SelfParamNode::accept(TreeVisitor& visitor) {
  if (this->self) {
    this->self->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ShorthandSelfNode::accept(TreeVisitor& visitor) {
  if (this->ampersand) {
    this->ampersand->accept(visitor);
  }
  if (this->mut) {
    this->mut->accept(visitor);
  }
  if (this->self) {
    this->self->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any TypedSelfNode::accept(TreeVisitor& visitor) {
  if (this->mut) {
    this->mut->accept(visitor);
  }
  if (this->self) {
    this->self->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any FunctionParamNode::accept(TreeVisitor& visitor) {
  if (this->pattern) {
    this->pattern->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any FunctionReturnTypeNode::accept(TreeVisitor& visitor) {
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalFunctionParametersNode::accept(TreeVisitor& visitor) {
  if (this->function_parameters) {
    this->function_parameters->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalCommaNode::accept(TreeVisitor& visitor) {
  if (this->comma) {
    this->comma->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CommaFunctionParamsNode::accept(TreeVisitor& visitor) {
  if (this->comma_function_params) {
    this->comma_function_params->accept(visitor);
  }
  if (this->comma) {
    this->comma->accept(visitor);
  }
  if (this->function_param) {
    this->function_param->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalFunctionReturnTypeNode::accept(TreeVisitor& visitor) {
  if (this->function_return_type) {
    this->function_return_type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any BlockExpressionOrSemicolonNode::accept(TreeVisitor& visitor) {
  if (this->block_expression) {
    this->block_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StructNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->optional_struct_fields) {
    this->optional_struct_fields->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StructFieldsNode::accept(TreeVisitor& visitor) {
  if (this->struct_field) {
    this->struct_field->accept(visitor);
  }
  if (this->comma_struct_fields) {
    this->comma_struct_fields->accept(visitor);
  }
  if (this->optional_comma) {
    this->optional_comma->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StructFieldNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalStructFieldsNode::accept(TreeVisitor& visitor) {
  if (this->struct_fields) {
    this->struct_fields->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CommaStructFieldsNode::accept(TreeVisitor& visitor) {
  if (this->comma_struct_fields) {
    this->comma_struct_fields->accept(visitor);
  }
  if (this->comma) {
    this->comma->accept(visitor);
  }
  if (this->struct_field) {
    this->struct_field->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any EnumerationNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->optional_enum_variants) {
    this->optional_enum_variants->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any EnumVariantsNode::accept(TreeVisitor& visitor) {
  if (this->enum_variant) {
    this->enum_variant->accept(visitor);
  }
  if (this->comma_enum_variants) {
    this->comma_enum_variants->accept(visitor);
  }
  if (this->optional_comma) {
    this->optional_comma->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any EnumVariantNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalEnumVariantsNode::accept(TreeVisitor& visitor) {
  if (this->enum_variants) {
    this->enum_variants->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CommaEnumVariantsNode::accept(TreeVisitor& visitor) {
  if (this->comma_enum_variants) {
    this->comma_enum_variants->accept(visitor);
  }
  if (this->comma) {
    this->comma->accept(visitor);
  }
  if (this->enum_variant) {
    this->enum_variant->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ConstantItemNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any TraitNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->items) {
    this->items->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ImplementationNode::accept(TreeVisitor& visitor) {
  if (this->implementation) {
    this->implementation->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any InherentImplNode::accept(TreeVisitor& visitor) {
  if (this->type) {
    this->type->accept(visitor);
  }
  if (this->items) {
    this->items->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any TraitImplNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  if (this->items) {
    this->items->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StatementNode::accept(TreeVisitor& visitor) {
  if (this->statement) {
    this->statement->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any LetStatementNode::accept(TreeVisitor& visitor) {
  if (this->pattern) {
    this->pattern->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ExpressionStatementNode::accept(TreeVisitor& visitor) {
  if (this->expression_statement) {
    this->expression_statement->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ExpressionNode::accept(TreeVisitor& visitor) {
  if (this->flow_control_expression) {
    this->flow_control_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any Unused1Node::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any BasicExpressionNode::accept(TreeVisitor& visitor) {
  if (this->basic_expression) {
    this->basic_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any LiteralExpressionNode::accept(TreeVisitor& visitor) {
  if (this->literal_expression) {
    this->literal_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any UnderscoreExpressionNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any GroupedExpressionNode::accept(TreeVisitor& visitor) {
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ArrayExpressionNode::accept(TreeVisitor& visitor) {
  if (this->optional_array_elements) {
    this->optional_array_elements->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalArrayElementsNode::accept(TreeVisitor& visitor) {
  if (this->array_elements) {
    this->array_elements->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ArrayElementsNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->other) {
    this->other->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CommaArrayElementsNode::accept(TreeVisitor& visitor) {
  if (this->comma_array_elements) {
    this->comma_array_elements->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any PathExpressionNode::accept(TreeVisitor& visitor) {
  if (this->path_in_expression) {
    this->path_in_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StructExpressionNode::accept(TreeVisitor& visitor) {
  if (this->path_in_expression) {
    this->path_in_expression->accept(visitor);
  }
  if (this->optional_struct_expr_fields) {
    this->optional_struct_expr_fields->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalStructExprFieldsNode::accept(TreeVisitor& visitor) {
  if (this->struct_expr_fields) {
    this->struct_expr_fields->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StructExprFieldsNode::accept(TreeVisitor& visitor) {
  if (this->struct_expr_field) {
    this->struct_expr_field->accept(visitor);
  }
  if (this->comma_struct_expr_fields) {
    this->comma_struct_expr_fields->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CommaStructExprFieldsNode::accept(TreeVisitor& visitor) {
  if (this->comma_struct_expr_fields) {
    this->comma_struct_expr_fields->accept(visitor);
  }
  if (this->comma) {
    this->comma->accept(visitor);
  }
  if (this->struct_expr_field) {
    this->struct_expr_field->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StructExprFieldNode::accept(TreeVisitor& visitor) {
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any PostfixExpressionNode::accept(TreeVisitor& visitor) {
  if (this->postfix_expression) {
    this->postfix_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any MethodCallExpressionNode::accept(TreeVisitor& visitor) {
  if (this->postfix_expression) {
    this->postfix_expression->accept(visitor);
  }
  if (this->path_expr_segment) {
    this->path_expr_segment->accept(visitor);
  }
  if (this->optional_call_params) {
    this->optional_call_params->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OptionalCallParamsNode::accept(TreeVisitor& visitor) {
  if (this->call_params) {
    this->call_params->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CallParamsNode::accept(TreeVisitor& visitor) {
  if (this->expression) {
    this->expression->accept(visitor);
  }
  if (this->comma_call_params) {
    this->comma_call_params->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CommaCallParamsNode::accept(TreeVisitor& visitor) {
  if (this->comma_call_params) {
    this->comma_call_params->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any FieldExpressionNode::accept(TreeVisitor& visitor) {
  if (this->postfix_expression) {
    this->postfix_expression->accept(visitor);
  }
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CallExpressionNode::accept(TreeVisitor& visitor) {
  if (this->postfix_expression) {
    this->postfix_expression->accept(visitor);
  }
  if (this->optional_call_params) {
    this->optional_call_params->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any IndexExpressionNode::accept(TreeVisitor& visitor) {
  if (this->postfix_expression) {
    this->postfix_expression->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any UnaryOperatorExpressionNode::accept(TreeVisitor& visitor) {
  if (this->unary_operator_expression) {
    this->unary_operator_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any BorrowExpressionNode::accept(TreeVisitor& visitor) {
  if (this->ampersands) {
    this->ampersands->accept(visitor);
  }
  if (this->mut) {
    this->mut->accept(visitor);
  }
  if (this->unary_operator_expression) {
    this->unary_operator_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any DereferenceExpressionNode::accept(TreeVisitor& visitor) {
  if (this->unary_operator_expression) {
    this->unary_operator_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any NegationExpressionNode::accept(TreeVisitor& visitor) {
  if (this->operator_) {
    this->operator_->accept(visitor);
  }
  if (this->unary_operator_expression) {
    this->unary_operator_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any TypeCastExpressionNode::accept(TreeVisitor& visitor) {
  if (this->unary_operator_expression) {
    this->unary_operator_expression->accept(visitor);
  }
  if (this->type_cast_expression) {
    this->type_cast_expression->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any MultiplicativeOperatorExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->operator_) {
    this->operator_->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any AdditiveOperatorExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->operator_) {
    this->operator_->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ShiftOperatorExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->operator_) {
    this->operator_->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any AndExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any XorExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any OrExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ComparisonOperatorExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->operator_) {
    this->operator_->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any LazyAndExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any LazyOrExpressionNode::accept(TreeVisitor& visitor) {
  if (this->first_expression) {
    this->first_expression->accept(visitor);
  }
  if (this->second_expression) {
    this->second_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any AssignmentExpressionNode::accept(TreeVisitor& visitor) {
  if (this->assignment_expression) {
    this->assignment_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any SimpleAssignmentExpressionNode::accept(TreeVisitor& visitor) {
  if (this->left_expression) {
    this->left_expression->accept(visitor);
  }
  if (this->right_expression) {
    this->right_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any CompoundAssignmentExpressionNode::accept(TreeVisitor& visitor) {
  if (this->left_expression) {
    this->left_expression->accept(visitor);
  }
  if (this->operator_) {
    this->operator_->accept(visitor);
  }
  if (this->right_expression) {
    this->right_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any FlowControlExpressionNode::accept(TreeVisitor& visitor) {
  if (this->flow_control_expression) {
    this->flow_control_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ContinueExpressionNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any BreakExpressionNode::accept(TreeVisitor& visitor) {
  if (this->flow_control_expression) {
    this->flow_control_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ReturnExpressionNode::accept(TreeVisitor& visitor) {
  if (this->flow_control_expression) {
    this->flow_control_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ExpressionWithBlockNode::accept(TreeVisitor& visitor) {
  if (this->expression_with_block) {
    this->expression_with_block->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any BlockExpressionNode::accept(TreeVisitor& visitor) {
  if (this->statements) {
    this->statements->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any StatementsNode::accept(TreeVisitor& visitor) {
  if (this->statements) {
    this->statements->accept(visitor);
  }
  if (this->statement) {
    this->statement->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any LoopExpressionNode::accept(TreeVisitor& visitor) {
  if (this->loop_expression) {
    this->loop_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any InfiniteLoopExpressionNode::accept(TreeVisitor& visitor) {
  if (this->block_expression) {
    this->block_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any PredicateLoopExpressionNode::accept(TreeVisitor& visitor) {
  if (this->conditions) {
    this->conditions->accept(visitor);
  }
  if (this->block_expression) {
    this->block_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any IfExpressionNode::accept(TreeVisitor& visitor) {
  if (this->conditions) {
    this->conditions->accept(visitor);
  }
  if (this->block_expression) {
    this->block_expression->accept(visitor);
  }
  if (this->else_expression) {
    this->else_expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ConditionsNode::accept(TreeVisitor& visitor) {
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any PatternNode::accept(TreeVisitor& visitor) {
  if (this->pattern) {
    this->pattern->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any IdentifierPatternNode::accept(TreeVisitor& visitor) {
  if (this->ref) {
    this->ref->accept(visitor);
  }
  if (this->mut) {
    this->mut->accept(visitor);
  }
  if (this->identifier) {
    this->identifier->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any WildcardPatternNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any ReferencePatternNode::accept(TreeVisitor& visitor) {
  if (this->ampersand) {
    this->ampersand->accept(visitor);
  }
  if (this->mut) {
    this->mut->accept(visitor);
  }
  if (this->pattern) {
    this->pattern->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any TypeNode::accept(TreeVisitor& visitor) {
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any TypePathNode::accept(TreeVisitor& visitor) {
  if (this->path_expr_segment) {
    this->path_expr_segment->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ReferenceTypeNode::accept(TreeVisitor& visitor) {
  if (this->mut) {
    this->mut->accept(visitor);
  }
  if (this->type) {
    this->type->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any ArrayTypeNode::accept(TreeVisitor& visitor) {
  if (this->type) {
    this->type->accept(visitor);
  }
  if (this->expression) {
    this->expression->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any UnitTypeNode::accept(TreeVisitor& visitor) {
  return visitor.visit(*this);
}

std::any PathInExpressionNode::accept(TreeVisitor& visitor) {
  if (this->path_expr_segment) {
    this->path_expr_segment->accept(visitor);
  }
  if (this->path_expr_segment2) {
    this->path_expr_segment2->accept(visitor);
  }
  return visitor.visit(*this);
}

std::any PathExprSegmentNode::accept(TreeVisitor& visitor) {
  if (this->path_expr_segment) {
    this->path_expr_segment->accept(visitor);
  }
  return visitor.visit(*this);
}
