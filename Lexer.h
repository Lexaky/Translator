#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Token.h"
#include "TokenTypesEnum.h"
#include "TokenTypes.h"
#include "AllowedSymbols.h"

using namespace std;

class Lexer {
public:
    vector<Token> extractTokens(string sourceCode);
private:
    vector<Token> tokens;
    
    string trim(const string& str);
    Token getToken(string str);
};