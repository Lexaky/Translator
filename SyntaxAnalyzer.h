#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>

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
	bool classCloseBracketExists = false;  //make local variables
	bool mainMethodExists = false;
	unordered_set<string> globalConst;
	unordered_set<string> globalVars;
	unordered_set<string> methods;

	Token getNextToken();
	void resetReceivedToken();
	void checkTokensAfterStaticKeyword(vector<Node> &nodes);
	Node parseMethod(string returnType, string identifier);
	void checkMainSignature();
	map<string, string> parseMethodParams();
	Node parseClassConst();
	Node parseClassVariable(string dataType, string identifier, string thirdTokenValue);
	//IfElseStatementNode parseIfElseStatement();
	//WhileStatementNode parseWhileStatement();
	bool checkTypeValueCompatibility(string dataType, TokenTypesEnum value);
	void checkDataType(const Token& token);
	void checkIdentifier(const Token& token);
};
