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
		mainMethodExists = true;
	}
	else if (token.getValue() == "final") {
		nodes.push_back(parseClassConst());
	}
	else if (token.getTokenType() == TokenTypesEnum::DATA_TYPE) {
		string dataType = token.getValue();
		token = getNextToken();
		string probobalIdentifier = token.getValue();	//identifier validation
		token = getNextToken();
		if (token.getValue() == "(") {
			nodes.push_back(parseMethod(accesIdentifier, dataType));
		}
		else {
			nodes.push_back(parseClassVariable(dataType, probobalIdentifier));
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
	return Node();
}

Node SyntaxAnalyzer::parseClassVariable(string dataType, string identifier)
{
	return Node();
}

