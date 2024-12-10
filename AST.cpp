#include "AST.h"

void AST::traverse(const std::shared_ptr<ASTNode>& node, int depth) const {
    if (!node) return;
    // ����������� �������� ����
    std::cout << std::string(depth * 2, ' ') << "Node Type: " << node->type;
    if (!node->value.empty()) {
        std::cout << ", Value: " << node->value;
    }
    std::cout << std::endl;

    // ����� ���� �������� �����
    for (const auto& child : node->children) {
        traverse(child, depth + 1);
    }
}

void AST::semanticAnalysis(const std::shared_ptr<ASTNode>& node, SymbolTable& table) const {
    if (!node) return;

    if (node->type == "variable_declaration") {
        // ��������� ���������� � ������� ��������
        if (!table.addSymbol(node->value, "int")) { // ������������ ��� int ��� �������
            std::cerr << "Error: Variable '" << node->value << "' already declared!" << std::endl;
        }
    }
    else if (node->type == "assignment") {
        // ���������, ���������� �� ����������
        if (!table.symbolExists(node->value)) {
            std::cerr << "Error: Variable '" << node->value << "' not declared!" << std::endl;
        }
    }
    else if (node->type == "operation") {
        // ��������� ������������ ����� ���������
        for (const auto& child : node->children) {
            if (!table.symbolExists(child->value)) {
                std::cerr << "Error: Variable '" << child->value << "' not declared!" << std::endl;
            }
        }
    }

    // ����������� ����� �������� �����
    for (const auto& child : node->children) {
        semanticAnalysis(child, table);
    }
}