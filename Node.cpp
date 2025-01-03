#include "Node.h"

void MainClassNode::setBody(vector<Node> body)
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

void ConstDeclarationNode::setValue(string value)
{
	this->value = value;
}

string ConstDeclarationNode::getType()
{
	return type;
}

void VariableDeclarationNode::setType(string type)
{
	this->type = type;
}

void VariableDeclarationNode::setName(string name)
{
	this->name = name;
}

void VariableDeclarationNode::setGlobal(bool isGlobal)
{
	this->isGlobal = isGlobal;
}

void VariableDeclarationNode::setInitialization(bool hasInitialization)
{
	this->hasInitialization = hasInitialization;
}

void VariableDeclarationNode::setValue(string value)
{
	this->value = value;
}

string VariableDeclarationNode::getType()
{
	return type;
}
