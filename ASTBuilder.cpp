#include "ASTBuilder.h"
#include "Token.h"
#include "TokenTypesEnum.h"

ASTBuilder::ASTBuilder(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}

Token ASTBuilder::currentToken() {
    if (currentIndex < tokens.size()) {
        return tokens[currentIndex];
    }
    throw std::runtime_error("Unexpected end of tokens.");
}

void ASTBuilder::consumeToken() {
    if (currentIndex < tokens.size()) {
        ++currentIndex;
    }
    else {
        throw std::runtime_error("Cannot consume token beyond token stream.");
    }
}

std::shared_ptr<ASTNode> ASTBuilder::parseStatement() {
    if (isEnd()) return nullptr;

    // Обработка модификаторов
    std::shared_ptr<ASTNode> modifierNode = parseModifier();
    if (modifierNode) {
        auto statementNode = std::make_shared<ASTNode>("Statement", "ModifiedStatement");
        statementNode->addChild(modifierNode);

        // Парсим следующий компонент, например, декларацию класса
        auto classNode = parseStatement();  // Пример, можно уточнить, что парсить
        if (classNode) {
            statementNode->addChild(classNode);
        }
        return statementNode;
    }

    // Обработка других типов выражений
    Token token = peekToken();
    if (token.getValue() == "class") {
        auto node = std::make_shared<ASTNode>("ClassDeclaration", "Main");
        consumeToken();

        token = peekToken();
        if (token.getTokenType() == TokenTypesEnum::IDENTIFIER) {
            node->addChild(std::make_shared<ASTNode>("ClassName", token.getValue()));
            consumeToken();
        }
        return node;
    }

    return nullptr;
}

std::shared_ptr<ASTNode> ASTBuilder::parsePackageDeclaration() {
    consumeToken(); // Consume 'package'
    if (currentToken().getTokenType() != UNKNOWN) {
        throw std::runtime_error("Expected identifier after 'package'.");
    }
    Token packageName = currentToken();
    consumeToken();

    if (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != ";") {
        throw std::runtime_error("Expected ';' after package declaration.");
    }
    consumeToken();

    return std::make_shared<ASTNode>("package_declaration", packageName.getValue());
}

std::shared_ptr<ASTNode> ASTBuilder::parseClassDeclaration() {
    consumeToken(); // Consume 'class'
    if (currentToken().getTokenType() != IDENTIFIER) {
        throw std::runtime_error("Expected class name after 'class'.");
    }
    Token className = currentToken();
    consumeToken();

    if (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != "{") {
        throw std::runtime_error("Expected '{' after class declaration.");
    }
    consumeToken();

    auto classNode = std::make_shared<ASTNode>("class_declaration", className.getValue());

    while (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != "}") {
        classNode->addChild(parseStatement());
    }

    consumeToken(); // Consume '}'
    return classNode;
}

std::shared_ptr<ASTNode> ASTBuilder::parseMethodDeclaration() {
    consumeToken(); // Skip access modifiers
    if (currentToken().getTokenType() != DATA_TYPE) {
        throw std::runtime_error("Expected return type in method declaration.");
    }
    Token returnType = currentToken();
    consumeToken();

    if (currentToken().getTokenType() != IDENTIFIER) {
        throw std::runtime_error("Expected method name.");
    }
    Token methodName = currentToken();
    consumeToken();

    if (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != "(") {
        throw std::runtime_error("Expected '(' in method declaration.");
    }
    consumeToken();

    // Parse parameters (simplified for now)
    while (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != ")") {
        consumeToken();
    }
    consumeToken(); // Consume ')'

    if (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != "{") {
        throw std::runtime_error("Expected '{' in method declaration.");
    }

    consumeToken();
    auto methodNode = std::make_shared<ASTNode>("method_declaration", methodName.getValue());
    methodNode->addChild(parseBlock());
    return methodNode;
}

std::shared_ptr<ASTNode> ASTBuilder::parseVariableDeclaration() {
    Token typeToken = currentToken();
    consumeToken();

    if (currentToken().getTokenType() != IDENTIFIER) {
        throw std::runtime_error("Expected identifier in variable declaration.");
    }
    Token nameToken = currentToken();
    consumeToken();

    auto varNode = std::make_shared<ASTNode>("variable_declaration", nameToken.getValue());

    if (currentToken().getTokenType() == OPERATOR && currentToken().getValue() == "=") {
        consumeToken();
        varNode->addChild(parseExpression());
    }

    if (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != ";") {
        throw std::runtime_error("Expected ';' in variable declaration.");
    }
    consumeToken();

    return varNode;
}

std::shared_ptr<ASTNode> ASTBuilder::parseAssignment() {
    Token nameToken = currentToken();
    consumeToken();

    if (currentToken().getTokenType() != OPERATOR) {
        throw std::runtime_error("Expected operator in assignment.");
    }
    Token operatorToken = currentToken();
    consumeToken();

    auto assignNode = std::make_shared<ASTNode>("assignment", nameToken.getValue());
    assignNode->addChild(parseExpression());

    if (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != ";") {
        throw std::runtime_error("Expected ';' in assignment.");
    }
    consumeToken();

    return assignNode;
}

std::shared_ptr<ASTNode> ASTBuilder::parseExpression() {
    Token token = currentToken();
    if (token.getTokenType() == IDENTIFIER || token.getTokenType() == STRING || token.getTokenType() == NUMBER) {
        consumeToken();
        return std::make_shared<ASTNode>("constant", token.getValue());
    }
    throw std::runtime_error("Unexpected token in expression: " + token.getValue());
}

std::shared_ptr<ASTNode> ASTBuilder::parseBlock() {
    auto blockNode = std::make_shared<ASTNode>("block");
    while (currentToken().getTokenType() != SPECIAL_SYMBOL || currentToken().getValue() != "}") {
        blockNode->addChild(parseStatement());
    }
    consumeToken(); // Consume '}'
    return blockNode;
}

std::shared_ptr<ASTNode> ASTBuilder::parseModifier() {
    std::cout << "Entering parseModifier()" << std::endl; // Отладочная строка

    if (isEnd()) {
        std::cout << "parseModifier(): End of tokens reached" << std::endl; // Отладочная строка
        return nullptr;
    }

    Token token = peekToken();
    std::string tokenValue = token.getValue();
    std::cout << "LOOK FOR TOKEN IN PARSE MODIFIER: " << tokenValue << "\n";
    // Список известных модификаторов
    static const std::vector<std::string> modifiers = {
        "public", "private", "protected", "static"
    };

    if (std::find(modifiers.begin(), modifiers.end(), tokenValue) != modifiers.end()) {
        std::cout << "parseModifier(): Found modifier " << tokenValue << std::endl; // Отладочная строка
        consumeToken(); // Переход к следующему токену
        return std::make_shared<ASTNode>("Modifier", tokenValue);
    }

    std::cout << "parseModifier(): No modifier found, returning nullptr" << std::endl; // Отладочная строка
    return nullptr;
}


std::shared_ptr<AST> ASTBuilder::buildAST() {
    auto root = std::make_shared<ASTNode>("program");

    while (currentIndex < tokens.size()) {
        std::cout << "LOOK FOR TOKEN: " << currentToken().getValue() << "\n";
        if (currentToken().getValue() == "package") {
            root->addChild(parsePackageDeclaration());
        }
        else if (currentToken().getValue() == "class") {
            root->addChild(parseClassDeclaration());
        }
        else if (currentToken().getValue() == "public") {
            root->addChild(parseStatement());
        }
        else {
            throw std::runtime_error("Unexpected token: " + currentToken().getValue());
        }
    }

    return std::make_shared<AST>(root);
}