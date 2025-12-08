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
  if (child) {
    return child->accept(*this);
  }
  return std::any();
}

void DebugTreeVisitor::visit_children(const std::vector<std::unique_ptr<TreeNode>>& children) {
  for (const auto& child : children) {
    visit_child(child.get());
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
  visit_children(node.items);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ItemNode& node) {
  print_node_start("ItemNode");
  visit_child(node.item.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionNode& node) {
  print_node_start("FunctionNode");
  print_indent();
  out << "identifier: \"test\"" << std::endl;
  out << "before visit_child optional_const" << std::endl;
  visit_child(node.optional_const.get());
  visit_child(node.optional_function_parameters.get());
  visit_child(node.optional_function_return_type.get());
  visit_child(node.block_expression_or_semicolon.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalConstNode& node) {
  out << "visiting OptionalConstNode" << std::endl;
  if (!node.value.empty()) {
    print_node_with_value("OptionalConstNode", node.value);
  } else {
    print_node_start("OptionalConstNode");
    print_indent();
    out << "(empty)" << std::endl;
    print_node_end();
  }
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionParametersNode& node) {
  print_node_start("FunctionParametersNode");
  visit_child(node.self_param.get());
  visit_child(node.optional_comma.get());
  visit_children(node.function_params);
  visit_child(node.comma_function_params.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(SelfParamNode& node) {
  print_node_start("SelfParamNode");
  visit_child(node.shorthand_self.get());
  visit_child(node.typed_self.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ShorthandSelfNode& node) {
  print_node_start("ShorthandSelfNode");
  print_indent();
  out << "ampersand: \"" << node.ampersand << "\"" << std::endl;
  print_indent();
  out << "mut: \"" << node.mut << "\"" << std::endl;
  print_indent();
  out << "self: \"" << node.self << "\"" << std::endl;
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TypedSelfNode& node) {
  print_node_start("TypedSelfNode");
  print_indent();
  out << "mut: \"" << node.mut << "\"" << std::endl;
  print_indent();
  out << "self: \"" << node.self << "\"" << std::endl;
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionParamNode& node) {
  print_node_start("FunctionParamNode");
  visit_child(node.pattern.get());
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FunctionReturnTypeNode& node) {
  print_node_start("FunctionReturnTypeNode");
  visit_child(node.type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalFunctionParametersNode& node) {
  print_node_start("OptionalFunctionParametersNode");
  visit_child(node.function_parameters.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalCommaNode& node) {
  if (!node.value.empty()) {
    print_node_with_value("OptionalCommaNode", node.value);
  } else {
    print_node_start("OptionalCommaNode");
    print_indent();
    out << "(empty)" << std::endl;
    print_node_end();
  }
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaFunctionParamsNode& node) {
  print_node_start("CommaFunctionParamsNode");
  visit_children(node.function_params);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(OptionalFunctionReturnTypeNode& node) {
  print_node_start("OptionalFunctionReturnTypeNode");
  visit_child(node.function_return_type.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(BlockExpressionOrSemicolonNode& node) {
  print_node_start("BlockExpressionOrSemicolonNode");
  if (!node.semicolon.empty()) {
    print_indent();
    out << "semicolon: \"" << node.semicolon << "\"" << std::endl;
  }
  visit_child(node.block_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructNode& node) {
  print_node_start("StructNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
  visit_child(node.optional_struct_fields.get());
  if (!node.semicolon.empty()) {
    print_indent();
    out << "semicolon: \"" << node.semicolon << "\"" << std::endl;
  }
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructFieldsNode& node) {
  print_node_start("StructFieldsNode");
  visit_children(node.struct_fields);
  visit_child(node.optional_comma.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructFieldNode& node) {
  print_node_start("StructFieldNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
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
  visit_children(node.struct_fields);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(EnumerationNode& node) {
  print_node_start("EnumerationNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
  visit_child(node.optional_enum_variants.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(EnumVariantsNode& node) {
  print_node_start("EnumVariantsNode");
  visit_children(node.enum_variants);
  visit_child(node.optional_comma.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(EnumVariantNode& node) {
  print_node_start("EnumVariantNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
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
  visit_children(node.enum_variants);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ConstantItemNode& node) {
  print_node_start("ConstantItemNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
  visit_child(node.type.get());
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(TraitNode& node) {
  print_node_start("TraitNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
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
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
  visit_child(node.type.get());
  visit_child(node.items.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StatementNode& node) {
  print_node_start("StatementNode");
  if (!node.semicolon.empty()) {
    print_indent();
    out << "semicolon: \"" << node.semicolon << "\"" << std::endl;
  }
  visit_child(node.item.get());
  visit_child(node.let_statement.get());
  visit_child(node.expression_statement.get());
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
  visit_child(node.flow_control_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(Unused1Node& node) {
  print_node_start("Unused1Node");
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
  if (!node.true_keyword.empty()) {
    print_indent();
    out << "true_keyword: \"" << node.true_keyword << "\"" << std::endl;
  }
  if (!node.false_keyword.empty()) {
    print_indent();
    out << "false_keyword: \"" << node.false_keyword << "\"" << std::endl;
  }
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
  visit_children(node.expressions);
  visit_child(node.optional_comma.get());
  visit_child(node.expression.get());
  visit_child(node.size_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaArrayElementsNode& node) {
  print_node_start("CommaArrayElementsNode");
  visit_children(node.expressions);
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
  visit_children(node.struct_expr_fields);
  visit_child(node.optional_comma.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaStructExprFieldsNode& node) {
  print_node_start("CommaStructExprFieldsNode");
  visit_children(node.struct_expr_fields);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StructExprFieldNode& node) {
  print_node_start("StructExprFieldNode");
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
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
  visit_children(node.expressions);
  visit_child(node.optional_comma.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(CommaCallParamsNode& node) {
  print_node_start("CommaCallParamsNode");
  visit_children(node.expressions);
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(FieldExpressionNode& node) {
  print_node_start("FieldExpressionNode");
  visit_child(node.postfix_expression.get());
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
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
  print_indent();
  out << "ampersand: \"" << node.ampersand << "\"" << std::endl;
  print_indent();
  out << "mut: \"" << node.mut << "\"" << std::endl;
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
  print_indent();
  out << "operator_: \"" << node.operator_ << "\"" << std::endl;
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
  print_indent();
  out << "operator_: \"" << node.operator_ << "\"" << std::endl;
  visit_child(node.multiplicative_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(AdditiveOperatorExpressionNode& node) {
  print_node_start("AdditiveOperatorExpressionNode");
  visit_child(node.multiplicative_operator_expression.get());
  print_indent();
  out << "operator_: \"" << node.operator_ << "\"" << std::endl;
  visit_child(node.additive_operator_expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(ShiftOperatorExpressionNode& node) {
  print_node_start("ShiftOperatorExpressionNode");
  visit_child(node.additive_operator_expression.get());
  print_indent();
  out << "operator_: \"" << node.operator_ << "\"" << std::endl;
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
  print_indent();
  out << "operator_: \"" << node.operator_ << "\"" << std::endl;
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
  print_indent();
  out << "operator_: \"" << node.operator_ << "\"" << std::endl;
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
  visit_child(node.statements.get());
  visit_child(node.expression.get());
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(StatementsNode& node) {
  print_node_start("StatementsNode");
  visit_children(node.statements);
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
  if (!node.ref.empty()) {
    print_indent();
    out << "ref: \"" << node.ref << "\"" << std::endl;
  }
  if (!node.mut.empty()) {
    print_indent();
    out << "mut: \"" << node.mut << "\"" << std::endl;
  }
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
  print_node_end();
  return std::any();
}

std::any DebugTreeVisitor::visit(WildcardPatternNode& node) {
  print_node_with_value("WildcardPatternNode", node.value);
  return std::any();
}

std::any DebugTreeVisitor::visit(ReferencePatternNode& node) {
  print_node_start("ReferencePatternNode");
  print_indent();
  out << "ampersand: \"" << node.ampersand << "\"" << std::endl;
  if (!node.mut.empty()) {
    print_indent();
    out << "mut: \"" << node.mut << "\"" << std::endl;
  }
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
  if (!node.mut.empty()) {
    print_indent();
    out << "mut: \"" << node.mut << "\"" << std::endl;
  }
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
  print_indent();
  out << "identifier: \"" << node.identifier << "\"" << std::endl;
  if (!node.self_keyword.empty()) {
    print_indent();
    out << "self_keyword: \"" << node.self_keyword << "\"" << std::endl;
  }
  print_node_end();
  return std::any();
}