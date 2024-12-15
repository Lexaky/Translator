#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Token.h"
#include "Node.h"
#include "TokenTypes.h"

using namespace std;

class SyntaxAnalyzer {
public:
	explicit SyntaxAnalyzer(const std::vector<Token>& tokens);
	MainClassNode buildAst();
private:
	vector<Token> tokens;
	int index = 0;
	bool classCloseBracketExists = false;
	bool mainMethodExists = false;

	Token getNextToken();
	void checkTokensAfterStaticKeyword(string accessIdentifier, vector<Node> &nodes);
	Node parseMethod(string accessIdentifier, string returnType);
	Node parseClassConst();
	Node parseClassVariable(string dataType, string identifier);
	//IfElseStatementNode parseIfElseStatement();
	//WhileStatementNode parseWhileStatement();
};
