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
			token = getNextToken();
			if (token.getValue() == "static") {
				checkTokensAfterStaticKeyword(nodes);
			}
			else {
				string message = "Expected keyword 'static', but was - " + token.getValue();
				cout << message;
				exit(1);
			}
		}
		else if (token.getValue() == "static") {
			checkTokensAfterStaticKeyword(nodes);
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
 	Token token = tokens.at(index++);
	if (token.getTokenType() == TokenTypesEnum::END_OF_FILE && !classCloseBracketExists) {
		string message = "Reached end of file. Class doesn't have close bracket '}'";
		cout << message;
		exit(1);
	}
	return token;
}

void SyntaxAnalyzer::resetReceivedToken()
{
	--index;
}

void SyntaxAnalyzer::checkTokensAfterStaticKeyword(vector<Node>& nodes)
{
	Token token = getNextToken();
	if (token.getValue() == "void") {
		token = getNextToken();
		checkIdentifier(token);
		if (token.getValue() == "main") mainMethodExists = true;
		string identifier = token.getValue();
		token = getNextToken();
		if (token.getValue() != "(") {
			string message = "Expected '(', but was - " + token.getValue();
			cout << message;
			exit(1);
		}
		nodes.push_back(parseMethod("void", identifier));
	}
	else if (token.getValue() == "final") {
		nodes.push_back(parseClassConst());
	}
	else if (token.getTokenType() == TokenTypesEnum::DATA_TYPE) {
		string dataType = token.getValue();
		token = getNextToken();
		checkIdentifier(token);
		string identifier = token.getValue();
		token = getNextToken();
		if (token.getValue() == "(") {
			nodes.push_back(parseMethod(dataType, identifier));
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

Node SyntaxAnalyzer::parseMethod(string returnType, string identifier)
{
	if (methods.count(identifier)) {	//Should add parameters check (method name isn't the only one condition)
		string message = "Method '" + identifier + "' has already been declared";
		cout << message;
		exit(1);
	}
	methods.insert(identifier);
	MethodDeclarationNode node;
	node.setReturnType(returnType);
	node.setName(identifier);

	if (identifier == "main") {
		checkMainSignature();
		//Set default params
	}
	else node.setParams(parseMethodParams());

	Token token = getNextToken();
	if (token.getValue() != "{") {
		string message = "Expected '{', but was - " + token.getValue();
		cout << message;
		exit(1);
	}

	token = getNextToken();
	while (token.getValue() != "}" && token.getTokenType() != TokenTypesEnum::END_OF_FILE) {
		string tokenValue = token.getValue();
		//Declaration
		if (token.getTokenType() == TokenTypesEnum::DATA_TYPE) {

		}
		//Const declaration
		if (tokenValue == "final") {

		}
		if (token.getTokenType() == TokenTypesEnum::IDENTIFIER) {
			token = getNextToken();
			//Method invocation
			if (token.getValue() == "(") {

			}
		}
		if (tokenValue == "for" || tokenValue == "while") {

		}
		if (tokenValue == "if") {

		}
		
		if (tokenValue == "System.out.println") {

		}

		token = getNextToken();
	}
	if (token.getValue() != "}") {
		string message = "Expected '}' for method body";
		cout << message;
		exit(1);
	}
	
	return node;
}

void SyntaxAnalyzer::checkMainSignature()
{
	Token token = getNextToken();
	if (token.getValue() != "String") {
		string message = "Expected 'String' in method main signature but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getValue() != "[") {
		string message = "Expected '[' in method main signature but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getValue() != "]") {
		string message = "Expected ']' in method main signature but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getTokenType() != TokenTypesEnum::IDENTIFIER) {
		string message = "Expected identifier for String[] in method main signature but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	if (token.getValue() != ")") {
		string message = "Expected ')' in method main signature but was - " + token.getValue();
		cout << message;
		exit(1);
	}
}

map<string, string> SyntaxAnalyzer::parseMethodParams()
{
	Token token = getNextToken();
	map<string, string> params;
	while (token.getValue() != ")" && token.getTokenType() != TokenTypesEnum::END_OF_FILE) {
		checkDataType(token);
		const string dataType = token.getValue();
		token = getNextToken();
		checkIdentifier(token);
		const string identifier = token.getValue();
		if (params.count(identifier)) {
			string message = "Variable '" + identifier + "' has already been declared";
			cout << message;
			exit(1);
		}
		params[identifier] = dataType;
		token = getNextToken();
		if (token.getValue() == ")") break;
		if (token.getValue() != ",") {
			string message = "Expected comma but was - " + token.getValue();
			cout << message;
			exit(1);
		}
		token = getNextToken();
	}
	if (token.getValue() != ")") {
		string message = "Expected ')' for method declaration";
		cout << message;
		exit(1);
	}
	return params;
}

Node SyntaxAnalyzer::parseClassConst()
{
	ConstDeclarationNode node;
	node.setGlobal(true);

	//Data type
	Token token = getNextToken();
	checkDataType(token);
	node.setType(token.getValue());

	//Identifier
	token = getNextToken();
	checkIdentifier(token);
	if (globalConst.count(token.getValue())) {
		string message = "Final variable '" + token.getValue() + "' has already been declared";
		cout << message;
		exit(1);
	}
	globalConst.insert(token.getValue());
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
	if (globalVars.count(identifier)) {
		string message = "Variable '" + identifier + "' has already been declared";
		cout << message;
		exit(1);
	}
	globalVars.insert(identifier);

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

void SyntaxAnalyzer::checkDataType(const Token& token)
{
	if (token.getTokenType() != TokenTypesEnum::DATA_TYPE) {
		string message = "Expected data type, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
}

void SyntaxAnalyzer::checkIdentifier(const Token& token)
{
	if (token.getTokenType() != TokenTypesEnum::IDENTIFIER) {
		string message = "Expected identifier, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
}

