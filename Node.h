#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Token.h"

using namespace std;

class Node {
public:
	virtual void print() const {};
};

class MainClassNode : public Node {
public:
	MainClassNode() {};
	void setBody(vector<shared_ptr<Node>> body);
	vector<shared_ptr<Node>> getBody() {
		return body;
	}
	void print() const override {
		cout << "Class body: [" << endl;
		for (size_t i = 0; i < body.size(); ++i) {
			body[i]->print();
		}
		cout << "]" << endl;
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
	void print() const override {
		cout << "Method: {" << endl;
		cout << "returnType: " << returnType << ",\n";
		cout << "name: " << name << ",\n";
		cout << "params: [\n";
		for (auto it = params.begin(); it != params.end(); ++it) {
			cout << it->first << "-" << it->second << ",\n";
		}
		cout << "],\n";
		cout << "body: [" << endl;
		for (size_t i = 0; i < body.size(); ++i) {
			body[i]->print();
		}
		cout << "]\n";
		cout << "}," << endl;
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
	void print() const override {
		cout << "Sout: {" << endl;
		cout << "params: [\n";
		for (size_t i = 0; i < params.size(); ++i) {
			cout << "'" << params.at(i).getValue() << "', ";
		}
		cout << "\n]\n";
		cout << "}," << endl;
	}
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
	void print() const override {
		cout << "MethodCall: [" << endl;
		for (size_t i = 0; i < params.size(); ++i) {
			cout << "'" << params.at(i).getValue() << "', ";
		}
		cout << "\n";
		cout << "]," << endl;
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
	void print() const override {
		cout << "Const Declaration: {" << endl;
		cout << "type: " << type << ",\n";
		cout << "name: " << name << ",\n";
		cout << "isGlobal: " << isGlobal << ",\n";
		if (initializer.empty()) {
			cout << "value: " << value.getValue() << ",\n";
		}
		else {
			cout << "value: [" << endl;
			for (size_t i = 0; i < initializer.size(); ++i) {
				cout << initializer[i].getValue() << ", ";
			}
			cout << "\n]\n";
		}
		cout << "}," << endl;
	}
protected:
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

	void print() const override {
		cout << "Variable Declaration: {" << endl;
		cout << "type: " << type << ",\n";
		cout << "name: " << name << ",\n";
		cout << "isGlobal: " << isGlobal << ",\n";
		if (hasInitialization) {
			if (initializer.empty()) {
				cout << "value: " << value.getValue() << ",\n";
			}
			else {
				cout << "value: [" << endl;
				for (size_t i = 0; i < initializer.size(); ++i) {
					cout << initializer[i].getValue() << ", ";
				}
				cout << "\n]\n";
			}
		}
		cout << "}," << endl;
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
	void print() const override {
		cout << "Assignment: {" << endl;
		cout << "variable: '" << variableName << "',\n";
		cout << "rightSide: [\n";
		for (size_t i = 0; i < rightSide.size(); ++i) {
			cout << "'" << rightSide.at(i).getValue() << "', ";
		}
		cout << "\n]\n";
		cout << "}," << endl;
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
		return condition;
	}
	void print() const override {
		(isFor) ? cout << "For: {\n" : cout << "While: {\n";
		cout << "condition: [\n";
		for (size_t i = 0; i < condition.size(); ++i) {
			cout << condition.at(i).getValue() << ", ";
		}
		cout << "\n],\n";
		cout << "body: [" << endl;
		for (size_t i = 0; i < body.size(); ++i) {
			body[i]->print();
		}
		cout << "\n]\n";
		cout << "}," << endl;
	}
private:
	bool isFor;		//otherwise while
	vector<Token> condition;
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
	void print() const override {
		cout << "IfElse: {" << endl;
		cout << "condition: [\n";
		for (size_t i = 0; i < condition.size(); ++i) {
			cout << "'" << condition.at(i).getValue() << "', ";
		}
		cout << "],\n";
		cout << "trueBody: [\n";
		for (size_t i = 0; i < trueBody.size(); ++i) {
			trueBody[i]->print();
		}
		if (hasElseStatement) {
			cout << "\n],\n";
			cout << "falseBody: [\n";
			for (size_t i = 0; i < falseBody.size(); ++i) {
				falseBody[i]->print();
			}
		}
		cout << "]\n";
		cout << "}" << endl;
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
	void print() const override {
		cout << "ReturnStatement: [" << endl;
		for (size_t i = 0; i < result.size(); ++i) {
			cout << "'" << result.at(i).getValue() << "', ";
		}
		cout << "\n";
		cout << "]," << endl;
	}
private:
	vector<Token> result;
};
