#include "Node.h"

void MainClassNode::setBody(vector<shared_ptr<Node>> body)
{
	this->body = body;
}

void ConstDeclarationNode::setType(string type)
{
	this->type = type;
}

void ConstDeclarationNode::setName(string name)
{
	this->name = name;
}

void ConstDeclarationNode::setGlobal(bool isGlobal)
{
	this->isGlobal = isGlobal;
}

void ConstDeclarationNode::setValue(Token value)
{
	this->value = value;
}

void ConstDeclarationNode::setInitializer(vector<Token> initializer)
{
	this->initializer = initializer;
}

string ConstDeclarationNode::getType()
{
	return type;
}

void VariableDeclarationNode::setInitialization(bool hasInitialization)
{
	this->hasInitialization = hasInitialization;
}

void MethodDeclarationNode::setMain(bool main)
{
	this->isMain = main;
}

void MethodDeclarationNode::setReturnType(string type)
{
	this->returnType = type;
}

void MethodDeclarationNode::setName(string name)
{
	this->name = name;
}

void MethodDeclarationNode::setParams(map<string, string> params)
{
	this->params = params;
}

void MethodDeclarationNode::setBody(vector<shared_ptr<Node>> body)
{
	this->body = body;
}

void SoutNode::setParams(vector<Token> params)
{
	this->params = params;
}

void MethodCall::setMethodName(string methodName)
{
	this->methodName = methodName;
}

void MethodCall::setParams(vector<Token> params)
{
	this->params = params;
}

void AssignmentNode::setVariableName(string name)
{
	this->variableName = name;
}

void AssignmentNode::setRightSide(vector<Token> rightSide)
{
	this->rightSide = rightSide;
}

void CycleStatementNode::setFor(bool isFor)
{
	this->isFor = isFor;
}

void CycleStatementNode::setCondition(vector<Token> condition)
{
	this->conditiion = conditiion;
}

void CycleStatementNode::setBody(vector<shared_ptr<Node>> body)
{
	this->body = body;
}

void IfElseStatementNode::setCondition(vector<Token> condition)
{
	this->condition = condition;
}

void IfElseStatementNode::setTrueBody(vector<shared_ptr<Node>> body)
{
	this->trueBody = body;
}

void IfElseStatementNode::setHasElseStatement(bool flag)
{
	this->hasElseStatement = flag;
}

void IfElseStatementNode::setFalseBody(vector<shared_ptr<Node>> body)
{
	this->falseBody = body;
}

void ReturnStatementNode::setResult(vector<Token> result)
{
	this->result = result;
}
