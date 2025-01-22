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
	bool classCloseBracketExists = false;
	bool mainMethodExists = false;
	map<string, string> classAtr;
	map<string, vector<string>> methods;

	Token getNextToken();
	void resetReceivedToken();
	void checkTokensAfterStaticKeyword(vector<Node> &nodes);
	MethodDeclarationNode parseMethod(string returnType, string identifier);
	void checkMainSignature();
	map<string, string> parseMethodParams();
	vector<Node> parseBody(); //for method, if and cycles bodies
	MethodCall parseMethodCall(const string& methodName, const map<string, string>& methodVars);
	AssignmentNode parseAssignment(const string& varName);
	SoutNode parseSout();
	ConstDeclarationNode parseConstDeclaration(map<string, string>& earlierDeclaredVars);
	VariableDeclarationNode parseVariableDeclaration(map<string, string>& earlierDeclaredVars, string dataType, string identifier);
	void parseDeclarationAssignment(ConstDeclarationNode& node);
	IfElseStatementNode parseIf();
	CycleStatementNode parseCycleStatement(const string& cycleKeyword);
	vector<Token> parseForCondition();
	vector<Token> parseWhileCondition();
	ReturnStatementNode parseReturnStatement();
	bool checkTypeValueCompatibility(const string& dataType, const TokenTypesEnum& value);
	void check(const string& expected, const Token& token);
	void checkTokenForConditionOrExpression(const Token& token);
	void checkDataType(const Token& token);
	void checkIdentifier(const Token& token);
	void checkUniqueClassAttribute(map<string, string>& earlierDeclaredVars, const string& identifier);
};
