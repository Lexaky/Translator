#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Token.h"

using namespace std;

class Node {};

class MainClassNode : public Node {
public:
	MainClassNode() {};
	void setBody(vector<Node> body);
private:
	vector<Node> body;
};

class MethodDeclarationNode : public Node {
public:
	MethodDeclarationNode() { isMain = false; };
	void setMain(bool main);
	void setReturnType(string type);
	void setName(string name);
	void setParams(map<string, string> params);
	void setBody(vector<Node> body);
private:
	bool isMain;
	string returnType;
	string name;
	map<string, string> params;
	vector<Node> body;
};

class SoutNode : public Node {
public:
	SoutNode() {};
	void setParams(vector<Token> params);
	vector<Token> getParams();
private:
	vector<Token> params;
};


class MethodCall : public Node {
public:
	MethodCall() {};
	void setMethodName(string methodName);
	void setParams(vector<Token> params);
private:
	string methodName;
	vector<Token> params;
};

class ConstDeclarationNode : public Node {
public:
	ConstDeclarationNode() {};
	void setType(string type);
	void setName(string name);
	void setGlobal(bool isGlobal);
	void setValue(Token value);
	void setInitializer(vector<Token> initializer);
	string getType();
private:
	string type;
	string name;
	bool isGlobal;
	Token value;
	vector<Token> initializer;
};

class VariableDeclarationNode : public ConstDeclarationNode {
public:
	VariableDeclarationNode() { };
	void setInitialization(bool hasInitialization);
private:
	bool hasInitialization;
};

class AssignmentNode : public Node {
public:
	AssignmentNode() {};
	void setVariableName(string name);
	void setRightSide(vector<Token> rightSide);
private:
	string variableName;
	vector<Token> rightSide;
};
//
//class UnaryOperator {
//public:
//	UnaryOperator();
//private:
//	string unOperator;
//	string operand;
//};
//
//class BinaryOperator {
//public:
//	BinaryOperator();
//private:
//	string left;
//	string binaryOperator;
//	string right;
//};
class CycleStatementNode : public Node {
public:
	CycleStatementNode() {};
	void setFor(bool isFor);
	void setCondition(vector<Token> condition);
	void setBody(vector<Node> body);
private:
	bool isFor;		//otherwise while
	vector<Token> conditiion;
	vector<Node> body;
};

class IfElseStatementNode : public Node {
public:
	IfElseStatementNode() { hasElseStatement = false; };
	void setCondition(vector<Token> condition);
	void setTrueBody(vector<Node> body);
	void setHasElseStatement(bool flag);
	void setFalseBody(vector<Node> body);
private:
	vector<Token> condition;
	vector<Node> trueBody;
	bool hasElseStatement;
	vector<Node> falseBody;
};

class ReturnStatementNode : public Node {
public:
	ReturnStatementNode() {};
	void setResult(vector<Token> result);
private:
	vector<Token> result;
};
