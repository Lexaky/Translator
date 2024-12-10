#pragma once
#include "AST.h"
#include <vector>
#include <stdexcept>
#include "Token.h"

class ASTBuilder {
private:
    std::vector<Token> tokens;
    size_t currentIndex;
    size_t currentTokenIndex;

    Token currentToken();
    void consumeToken();

    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parsePackageDeclaration();
    std::shared_ptr<ASTNode> parseClassDeclaration();
    std::shared_ptr<ASTNode> parseMethodDeclaration();
    std::shared_ptr<ASTNode> parseVariableDeclaration();
    std::shared_ptr<ASTNode> parseAssignment();
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseModifier();

public:
    explicit ASTBuilder(const std::vector<Token>& tokens);

    std::shared_ptr<AST> buildAST();
    bool isEnd() const { return currentTokenIndex >= tokens.size(); }
    Token peekToken() const { return isEnd() ? Token() : tokens[currentTokenIndex]; }
};
