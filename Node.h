#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Node {};

class MainClassNode : public Node {
public:
	MainClassNode() {};
	void setBody(vector<Node> body);
private:
	vector<Node> body;
};

//class MethodDeclarationNode : public Node {
//public:
//	MethodDeclarationNode();
//	void setReturnType(string type);
//	void setName(string name);
//	void setParams(vector<Node> params);
//	void setBody(vector<Node>);
//private:
//	string returnType;
//	string name;
//	vector<Node> params;
//	vector<Node> body;
//};
//
//class MethodCall : public Node {
//public:
//	MethodCall();
//	void setMethodName(string methodName);
//private:
//	string methodName;
//	vector<Node> params;
//};
//
//class VariableDeclarationNode : public Node {
//public:
//	VariableDeclarationNode();
//private:
//	string type;
//	string name;
//	bool hasInitialization;
//	Node initializer;
//};
//
//class ConstDeclarationNode : public Node {
//public:
//	ConstDeclarationNode();
//private:
//	string type;
//	string name;
//	Node initializer;
//};
//
//class AssignmentNode : public Node {
//public:
//	AssignmentNode();
//private:
//	string variableName;
//	string assignmentOperator;
//	Node rightSide;
//};
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
//
//class Condition {
//	//i dont know yet
//};
//
//class WhileStatementNode : public Node {
//public:
//	WhileStatementNode();
//	void setBody(vector<Node>);
//private:
//	Node condition;
//	vector<Node> body;
//};
//
//class ForStatementNode : public Node {
//public:
//	ForStatementNode();
//	void setBody(vector<Node>);
//private:
//	VariableDeclarationNode init;
//	Node condition;
//	UnaryOperator modification;
//	vector<Node> body;
//};
//
//class IfElseStatementNode : public Node {
//public:
//	IfElseStatementNode();
//private:
//	Node condition;
//	vector<Node> trueBody;
//	bool hasElseStatement;
//	vector<Node> falseBody;
//};
//
//class ReturnStatementNode : public Node {
//public:
//	ReturnStatementNode();
//	void setResult(Node result);
//private:
//	Node result;
//};
