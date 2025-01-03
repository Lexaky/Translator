#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const vector<Token>& tokens) : tokens(tokens) {}

MainClassNode SyntaxAnalyzer::buildAst()
{
	vector<Node> nodes;
	Token token = getNextToken();
	if (token.getValue() != "public") {
		string message = "Missing access modifier: public";
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getValue() != "class") {
		string message = "Missing keyword: class";
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getValue() != "Main") {
		string message = "Missing class name: Main";
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getValue() != "{") {
		string message = "Expected: {";
		cout << message;
		exit(1);
	}
	token = getNextToken();
	while (token.getTokenType() != TokenTypesEnum::END_OF_FILE) {
		if (token.getValue() == "}") {
			classCloseBracketExists = true;
			break;
		}
		if (token.getTokenType() == TokenTypesEnum::ACCESS_MODIFIER) {
			string accessModifier = token.getValue();
			token = getNextToken();
			if (token.getValue() == "static") {
				checkTokensAfterStaticKeyword(accessModifier, nodes);
			}
			else {
				string message = "Expected keyword 'static', but was - " + token.getValue();
				cout << message;
				exit(1);
			}
		}
		else if (token.getValue() == "static") {
			checkTokensAfterStaticKeyword("", nodes);
		}
		else {
			string message = "Expected access modifier or keyword 'static', but was - " + token.getValue();
			cout << message;
			exit(1);
		}
		token = getNextToken();
	}

	if (!classCloseBracketExists) {
		string message = "Expected: }";
		cout << message;
		exit(1);
	}
	if (!mainMethodExists) {
		string message = "Expected method main";
		cout << message;
		exit(1);
	}

	MainClassNode node;
	node.setBody(nodes);
	return node;
}

Token SyntaxAnalyzer::getNextToken()
{
 	return tokens.at(index++);
}

void SyntaxAnalyzer::checkTokensAfterStaticKeyword(string accesIdentifier, vector<Node>& nodes)
{
	Token token = getNextToken();
	if (token.getValue() == "void") {
		nodes.push_back(parseMethod(accesIdentifier, "void"));
		mainMethodExists = true; //add method identifier check
	}
	else if (token.getValue() == "final") {
		nodes.push_back(parseClassConst());
	}
	else if (token.getTokenType() == TokenTypesEnum::DATA_TYPE) {
		string dataType = token.getValue();
		token = getNextToken();
		if (token.getTokenType() != TokenTypesEnum::IDENTIFIER) {
			string message = "Expected identifier, but was - " + token.getValue();
			cout << message;
			exit(1);
		}
		string identifier = token.getValue();
		token = getNextToken();
		if (token.getValue() == "(") {
			nodes.push_back(parseMethod(accesIdentifier, dataType));
		}
		else {
			nodes.push_back(parseClassVariable(dataType, identifier, token.getValue()));
		}
	}
	else {
		string message = "Expected data type, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
}

Node SyntaxAnalyzer::parseMethod(string accesIdentifier, string returnType)
{
	return Node();
}

Node SyntaxAnalyzer::parseClassConst()
{
	ConstDeclarationNode node;
	node.setGlobal(true);

	//Data type
	Token token = getNextToken();
	if (token.getTokenType() != TokenTypesEnum::DATA_TYPE) {
		string message = "Expected data type, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	node.setType(token.getValue());

	//Identifier
	token = getNextToken();
	if (token.getTokenType() != TokenTypesEnum::IDENTIFIER) {
		string message = "Expected identifier, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	node.setName(token.getValue());
	
	//Assignment symbol
	token = getNextToken();
	if (token.getValue() != "=") {
		string message = "Expected assignment symbol, but was - " + token.getValue();
		cout << message;
		exit(1);
	}

	//Value or expression
	token = getNextToken();
	Token secondToken = getNextToken();
	//Value
	if (secondToken.getValue() == ";") {
		if (node.getType() == "String" && token.getTokenType() == TokenTypesEnum::STRING
			|| node.getType() == "char" && token.getTokenType() == TokenTypesEnum::CHAR
			|| node.getType() == "boolean" && token.getTokenType() == TokenTypesEnum::BOOLEAN) {
			node.setValue(token.getValue());
		}
		else if ((node.getType() == "int" || node.getType() == "long" || node.getType() == "short" || node.getType() == "byte")
			&& token.getTokenType() == TokenTypesEnum::INT) {
			node.setValue(token.getValue());
		}
		else if ((node.getType() == "float" || node.getType() == "double") && token.getTokenType() == TokenTypesEnum::FLOAT) {
			node.setValue(token.getValue());
		}
		else {
			string message = "Data type '" + node.getType() + "' doesn't match value - " + token.getValue();
			cout << message;
			exit(1);
		}
	}
	//Expression
	else {
		//Not implemented yet!!!!!!!!!!!!!!!!!!!!!!!!!!
		string message = "Expression parsing is not implemented";
		cout << message;
		exit(1);
	}

	return node;
}

//Refactor Ctrl + C / Ctrl + V
Node SyntaxAnalyzer::parseClassVariable(string dataType, string identifier, string thirdTokenValue)
{
	VariableDeclarationNode node;
	node.setGlobal(true);
	node.setType(dataType);
	node.setName(identifier);

	//Assignment symbol or semicolon
	if (thirdTokenValue == ";") {
		node.setInitialization(false);
		return node;
	}
	if (thirdTokenValue != "=") {
		string message = "Expected assignment symbol, but was - " + thirdTokenValue;
		cout << message;
		exit(1);
	}

	//Value or expression
	Token token = getNextToken();
	Token secondToken = getNextToken();
	//Value
	if (secondToken.getValue() == ";") {
		if (node.getType() == "String" && token.getTokenType() == TokenTypesEnum::STRING
			|| node.getType() == "char" && token.getTokenType() == TokenTypesEnum::CHAR
			|| node.getType() == "boolean" && token.getTokenType() == TokenTypesEnum::BOOLEAN) {
			node.setValue(token.getValue());
		}
		else if ((node.getType() == "int" || node.getType() == "long" || node.getType() == "short" || node.getType() == "byte")
			&& token.getTokenType() == TokenTypesEnum::INT) {
			node.setValue(token.getValue());
		}
		else if ((node.getType() == "float" || node.getType() == "double") && token.getTokenType() == TokenTypesEnum::FLOAT) {
			node.setValue(token.getValue());
		}
		else {
			string message = "Data type '" + node.getType() + "' doesn't match value - " + token.getValue();
			cout << message;
			exit(1);
		}
	}
	//Expression
	else {
		//Not implemented yet!!!!!!!!!!!!!!!!!!!!!!!!!!
		string message = "Expression parsing is not implemented";
		cout << message;
		exit(1);
	}

	return node;
}

