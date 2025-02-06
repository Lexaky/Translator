#pragma once
#include <vector>
#include "Token.h"

struct varsInProgram {
	std::string varName;
	std::string varType;
	bool inParameter;
	std::string area;
};

struct expressionsInProgram {
	std::vector <Token> expr;
	std::string area;
};

class SemanticAnalyzer
{
private:
	std::vector <varsInProgram> allVariables;
	std::vector <Token> tokens;
	std::vector <expressionsInProgram> eip;
	void fillAllVariables();
	void getAllExpressionsByVariablesAndReturn();
	void checkDivisionByZero();
	void checkTypeCompatibility();
	void checkVariableDeclaration();
public:
	SemanticAnalyzer(std::vector<Token> toks);
	void printAllVariables();
	void printAllExpressions();
	void doSemanticAnalysis();
};

