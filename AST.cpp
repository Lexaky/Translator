#include "AST.h"

void AST::traverse(const std::shared_ptr<ASTNode>& node, int depth) const {
    if (!node) return;
    // Отображение текущего узла
    std::cout << std::string(depth * 2, ' ') << "Node Type: " << node->type;
    if (!node->value.empty()) {
        std::cout << ", Value: " << node->value;
    }
    std::cout << std::endl;

    // Обход всех дочерних узлов
    for (const auto& child : node->children) {
        traverse(child, depth + 1);
    }
}

void AST::semanticAnalysis(const std::shared_ptr<ASTNode>& node, SymbolTable& table) const {
    if (!node) return;

    if (node->type == "variable_declaration") {
        // Добавляем переменную в таблицу символов
        if (!table.addSymbol(node->value, "int")) { // Используется тип int для примера
            std::cerr << "Error: Variable '" << node->value << "' already declared!" << std::endl;
        }
    }
    else if (node->type == "assignment") {
        // Проверяем, существует ли переменная
        if (!table.symbolExists(node->value)) {
            std::cerr << "Error: Variable '" << node->value << "' not declared!" << std::endl;
        }
    }
    else if (node->type == "operation") {
        // Проверяем корректность типов операндов
        for (const auto& child : node->children) {
            if (!table.symbolExists(child->value)) {
                std::cerr << "Error: Variable '" << child->value << "' not declared!" << std::endl;
            }
        }
    }

    // Рекурсивный обход дочерних узлов
    for (const auto& child : node->children) {
        semanticAnalysis(child, table);
    }
}