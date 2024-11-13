#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Token.h"
#include "TokenTypesEnum.h"
#include "TokenTypes.h"

class Lexer {
public:
    std::vector<Token> extractTokens(std::string sourceCode);
private:
    std::vector<Token> tokens;
    
    std::string trim(const std::string& str);
    Token getToken(std::string str);
};