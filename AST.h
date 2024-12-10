#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
class ASTNode {
public:
    std::string type;                     // Тип узла (e.g., "variable_declaration", "assignment")
    std::string value;                    // Значение узла, например, имя переменной
    std::vector<std::shared_ptr<ASTNode>> children; // Дочерние узлы

    explicit ASTNode(const std::string& type, const std::string& value = "")
        : type(type), value(value) {}

    void addChild(std::shared_ptr<ASTNode> child) {
        children.push_back(child);
    }
    void print(int depth = 0) const {
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        std::cout << type << ": " << value << std::endl;

        for (const auto& child : children) {
            child->print(depth + 1);
        }
    }
};

class SymbolTable {
public:
    std::unordered_map<std::string, std::string> symbols; // Имя переменной -> тип данных

    bool addSymbol(const std::string& name, const std::string& type) {
        if (symbols.find(name) != symbols.end()) return false; // Уже существует
        symbols[name] = type;
        return true;
    }

    bool symbolExists(const std::string& name) const {
        return symbols.find(name) != symbols.end();
    }

    std::string getSymbolType(const std::string& name) const {
        auto it = symbols.find(name);
        if (it != symbols.end()) return it->second;
        return "";
    }
};

class AST {
public:
    std::shared_ptr<ASTNode> root;

    explicit AST(std::shared_ptr<ASTNode> rootNode) : root(std::move(rootNode)) {}

    void traverse(const std::shared_ptr<ASTNode>& node, int depth = 0) const;
    void semanticAnalysis(const std::shared_ptr<ASTNode>& node, SymbolTable& table) const;
    void print() {
        root->print();
    }
};
