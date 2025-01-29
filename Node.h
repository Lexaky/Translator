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
	void setBody(vector<shared_ptr<Node>> body);
	vector<shared_ptr<Node>> getBody() {
		return body;
	}
private:
	vector<shared_ptr<Node>> body;
};

class MethodDeclarationNode : public Node {
public:
	MethodDeclarationNode() { isMain = false; };
	void setMain(bool main);
	void setReturnType(string type);
	void setName(string name);
	void setParams(map<string, string> params);
	void setBody(vector<shared_ptr<Node>> body);
	bool isMainMethod() {
		return isMain;
	}
	string getReturnType() {
		return returnType;
	}
	string getName() {
		return name;
	}
	map<string, string> getParams() {
		return params;
	}
	vector<shared_ptr<Node>> getBody() {
		return body;
	}
private:
	bool isMain;
	string returnType;
	string name;
	map<string, string> params;
	vector<shared_ptr<Node>> body;
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
	string getMethodName() {
		return methodName;
	}
	vector <Token> getTokens() {
		return params;
	}
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
	string getName() {
		return name;
	}
	bool isGlobalConst() {
		return isGlobal;
	}
	Token getValue() {
		return value;
	}
	vector<Token> getTokens() {
		return initializer;
	}
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
	bool isInit() {
		return hasInitialization;
	}
private:
	bool hasInitialization;
};

class AssignmentNode : public Node {
public:
	AssignmentNode() {};
	void setVariableName(string name);
	void setRightSide(vector<Token> rightSide);
	string getVariable() {
		return variableName;
	}
	vector <Token> getTokens() {
		return rightSide;
	}
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
	void setBody(vector<shared_ptr<Node>> body);
	vector<shared_ptr<Node>> getBody() {
		return body;
	}
	vector <Token> getTokens() {
		return conditiion;
	}
private:
	bool isFor;		//otherwise while
	vector<Token> conditiion;
	vector<shared_ptr<Node>> body;
};

class IfElseStatementNode : public Node {
public:
	IfElseStatementNode() { hasElseStatement = false; };
	void setCondition(vector<Token> condition);
	void setTrueBody(vector<shared_ptr<Node>> body);
	void setHasElseStatement(bool flag);
	void setFalseBody(vector<shared_ptr<Node>> body);
	vector <Token> getTokens() {
		return condition;
	}
	vector <shared_ptr<Node>> getBody() {
		return trueBody;
	}
	vector<shared_ptr<Node>> getFalseBody() {
		return falseBody;
	}
	bool isElse() {
		return hasElseStatement;
	}
private:
	vector<Token> condition;
	vector<shared_ptr<Node>> trueBody;
	bool hasElseStatement;
	vector<shared_ptr<Node>> falseBody;
};

class ReturnStatementNode : public Node {
public:
	ReturnStatementNode() {};
	void setResult(vector<Token> result);
	vector <Token> getTokens() {
		return result;
	}
private:
	vector<Token> result;
};
