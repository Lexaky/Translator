#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const vector<Token>& tokens) : tokens(tokens) {}

MainClassNode SyntaxAnalyzer::buildAst()
{
	vector<shared_ptr<Node>> nodes;
	Token token = getNextToken();
	check("public", token);
	token = getNextToken();
	check("class", token);
	token = getNextToken();
	check("Main", token);
	token = getNextToken();
	check("{", token);
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

void SyntaxAnalyzer::checkTokensAfterStaticKeyword(vector<shared_ptr<Node>>& nodes)
{
	Token token = getNextToken();
	if (token.getValue() == "void") {
		token = getNextToken();
		checkIdentifier(token);
		if (token.getValue() == "main") mainMethodExists = true;
		string identifier = token.getValue();
		token = getNextToken();
		check("(", token);
		auto node = make_shared<MethodDeclarationNode>();
		*node = parseMethod("void", identifier);
		nodes.push_back(node);
	}
	else if (token.getValue() == "final") {
		auto node = make_shared<ConstDeclarationNode>();
		*node = parseConstDeclaration(classAtr);
		node->setGlobal(true);
		nodes.push_back(node);
	}
	else if (token.getTokenType() == TokenTypesEnum::DATA_TYPE) {
		string dataType = token.getValue();
		token = getNextToken();
		checkIdentifier(token);
		string identifier = token.getValue();
		token = getNextToken();
		if (token.getValue() == "(") {
			auto node = make_shared<MethodDeclarationNode>();
			*node = parseMethod(dataType, identifier);
			nodes.push_back(node);
		}
		else {
			resetReceivedToken();
			auto node = make_shared<VariableDeclarationNode>();
			*node = parseVariableDeclaration(classAtr, dataType, identifier);
			node->setGlobal(true);
			nodes.push_back(node);
		}
	}
	else {
		string message = "Expected data type, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
}

MethodDeclarationNode SyntaxAnalyzer::parseMethod(string returnType, string identifier)
{
	if (methods.count(identifier)) {	//Should add parameters check (method name isn't the only one condition)
		string message = "Method '" + identifier + "' has already been declared";
		cout << message;
		exit(1);
	}
	MethodDeclarationNode node;
	node.setReturnType(returnType);
	node.setName(identifier);

	if (identifier == "main") {
		checkMainSignature();
		node.setMain(true);
		//Set default params
	}
	else {
		const map<string, string> params = parseMethodParams();
		node.setParams(params);

		vector<string> paramsDataTypes;
		for (auto it = params.begin(); it != params.end(); ++it) {
			paramsDataTypes.push_back(it->second);
		}
		methods[identifier] = paramsDataTypes;
	}

	Token token = getNextToken();
	check("{", token);

	vector<shared_ptr<Node>> body;
	parseBody(body);
	node.setBody(body);

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

void SyntaxAnalyzer::parseBody(vector<shared_ptr<Node>>& body)
{
	map<string, string> bodyVars;
	Token token = getNextToken();
	while (token.getValue() != "}") {
		string tokenValue = token.getValue();
		//Declaration
		if (token.getTokenType() == TokenTypesEnum::DATA_TYPE) {
			string dataType = token.getValue();
			token = getNextToken();
			checkIdentifier(token);
			auto node = make_shared<VariableDeclarationNode>();
			*node = parseVariableDeclaration(bodyVars, dataType, token.getValue());
			node->setGlobal(false);
			body.push_back(node);
		}
		//Const declaration
		else if (tokenValue == "final") {
			auto node = make_shared<ConstDeclarationNode>();
			*node = parseConstDeclaration(bodyVars);
			node->setGlobal(false);
			body.push_back(node);
		}
		else if (token.getTokenType() == TokenTypesEnum::IDENTIFIER) {
			const string name = token.getValue();
			token = getNextToken();
			//Method invocation
			if (token.getValue() == "(") {
				auto node = make_shared<MethodCall>();
				*node = parseMethodCall(name, bodyVars);
				body.push_back(node);
			}
			//var
			else {
				resetReceivedToken();
				auto node = make_shared<AssignmentNode>();
				*node = parseAssignment(name);
				body.push_back(node);
			}
		}
		else if (tokenValue == "for" || tokenValue == "while") {
			auto node = make_shared<CycleStatementNode>();
			*node = parseCycleStatement(tokenValue);
			body.push_back(node);
		}
		else if (tokenValue == "if") {
			auto node = make_shared<IfElseStatementNode>();
			*node = parseIf();
			body.push_back(node);
		}

		else if (tokenValue == "System.out.println") {
			auto node = make_shared<SoutNode>();
			*node = parseSout();
			body.push_back(node);
		}

		else if (tokenValue == "return") {
			auto node = make_shared<ReturnStatementNode>();
			*node = parseReturnStatement();
			body.push_back(node);
		}

		token = getNextToken();
	}
}

MethodCall SyntaxAnalyzer::parseMethodCall(const string& methodName, const map<string, string>& methodVars)
{
	if (!methods.count(methodName)) {
		string message = "Unknown identifier - " + methodName;
		cout << message;
		exit(1);
	}
	vector<string> expectedParamsDataTypes = methods.at(methodName);
	Token token = getNextToken();
	int paramIndex = 0;
	vector<Token> params;
	while (token.getValue() != ")") {	//for now doesn't handles expressions
		string expectedDataType = expectedParamsDataTypes.at(paramIndex++);
		if (token.getTokenType() == TokenTypesEnum::BOOLEAN || token.getTokenType() == TokenTypesEnum::CHAR
			|| token.getTokenType() == TokenTypesEnum::STRING || token.getTokenType() == TokenTypesEnum::FLOAT
			|| token.getTokenType() == TokenTypesEnum::INT) {
			checkTypeValueCompatibility(expectedDataType, token.getTokenType());
		}
		else {
			checkIdentifier(token);
			if (classAtr.count(token.getValue())) {
				string actualDataType = classAtr.at(token.getValue());
				if (actualDataType != expectedDataType) {
					string message = "Expected data type '" + expectedDataType + "', but was - " + actualDataType;
					cout << message;
					exit(1);
				}
			}
			else if (methodVars.count(token.getValue())) {
				string actualDataType = methodVars.at(token.getValue());
				if (actualDataType != expectedDataType) {
					string message = "Expected data type '" + expectedDataType + "', but was - " + actualDataType;
					cout << message;
					exit(1);
				}
			}
			else {
				string message = "Unknown identifier - " + token.getValue();
				cout << message;
				exit(1);
			}
		}
		params.push_back(token);
		token = getNextToken();
		check(",", token);
		token = getNextToken();
	}
	token = getNextToken();
	check(";", token);
	MethodCall node;
	node.setMethodName(methodName);
	node.setParams(params);
	return node;
}

AssignmentNode SyntaxAnalyzer::parseAssignment(const string& varName)
{
	AssignmentNode node;
	node.setVariableName(varName);
	Token token = getNextToken();
	vector<Token> expression;
	if (token.getValue() == "++" || token.getValue() == "--") {
		expression.push_back(token);
		token = getNextToken();
		check(";", token);
	}
	else {
		check("=", token);
		token = getNextToken();
		while (token.getValue() != ";") {
			if (token.getTokenType() != TokenTypesEnum::OPERATOR && token.getTokenType() != TokenTypesEnum::BOOLEAN
				&& token.getTokenType() != TokenTypesEnum::CHAR && token.getTokenType() != TokenTypesEnum::FLOAT
				&& token.getTokenType() != TokenTypesEnum::INT && token.getTokenType() != TokenTypesEnum::STRING
				&& token.getTokenType() != TokenTypesEnum::IDENTIFIER && token.getValue() != "("
				&& token.getValue() != ")") {
				string message = "Value of unexpected type - " + token.getValue();
				cout << message;
				exit(1);
			}
			expression.push_back(token);
			token = getNextToken();
		}
	}
	node.setRightSide(expression);
	return node;
}

SoutNode SyntaxAnalyzer::parseSout()
{
	Token token = getNextToken();
	check("(", token);
	token = getNextToken();
	vector<Token> soutTokens;
	while (token.getValue() != ")") {  //make validation more complicated
		checkTokenForConditionOrExpression(token);
		//check identifiers
		soutTokens.push_back(token);
		token = getNextToken();
	}
	token = getNextToken();
	check(";", token);
	SoutNode node;
	node.setParams(soutTokens);

	return node;
}

ConstDeclarationNode SyntaxAnalyzer::parseConstDeclaration(map<string, string>& earlierDeclaredVars)
{
	ConstDeclarationNode node;
	//Data type
	Token token = getNextToken();
	checkDataType(token);
	node.setType(token.getValue());

	//Identifier
	token = getNextToken();
	checkIdentifier(token);
	const string identifier = token.getValue();
	checkUniqueClassAttribute(earlierDeclaredVars, identifier);
	const string type = "const";
	earlierDeclaredVars[identifier] = type;
	node.setName(token.getValue());

	//Assignment symbol
	token = getNextToken();
	check("=", token);

	parseDeclarationAssignment(node);

	return node;
}

VariableDeclarationNode SyntaxAnalyzer::parseVariableDeclaration(map<string, string>& earlierDeclaredVars, string dataType, string identifier)
{
	checkUniqueClassAttribute(earlierDeclaredVars, identifier);
	const string type = "var";
	earlierDeclaredVars[identifier] = type;

	VariableDeclarationNode node;
	node.setType(dataType);
	node.setName(identifier);

	//Assignment symbol or semicolon
	Token token = getNextToken();
	if (token.getValue() == ";") {
		node.setInitialization(false);
		return node;
	}
	check("=", token);
	node.setInitialization(true);

	parseDeclarationAssignment(node);

	return node;
}

void SyntaxAnalyzer::parseDeclarationAssignment(ConstDeclarationNode& node)
{
	//Value or expression
	Token token = getNextToken();
	Token secondToken = getNextToken();
	//Value
	if (secondToken.getValue() == ";") {
		bool flag = checkTypeValueCompatibility(node.getType(), token.getTokenType());
		if (flag) {
			node.setValue(token);
		}
		else {
			string message = "Data type '" + node.getType() + "' doesn't match value - " + token.getValue();
			cout << message;
			exit(1);
		}
	}
	//Expression
	else {
		vector<Token> initializer;
		checkTokenForConditionOrExpression(secondToken);
		initializer.push_back(secondToken);
		token = getNextToken();
		while (token.getValue() != ";") {
			checkTokenForConditionOrExpression(token);
			initializer.push_back(token);
			token = getNextToken();
		}
		node.setInitializer(initializer);
	}
}

IfElseStatementNode SyntaxAnalyzer::parseIf()
{
	IfElseStatementNode node;
	node.setCondition(parseWhileCondition()); //same logic fow now
	Token token = getNextToken();
	check("{", token);
	vector<shared_ptr<Node>> trueBody;
	parseBody(trueBody);
	node.setTrueBody(trueBody);
	token = getNextToken();
	if (token.getValue() == "else") {
		node.setHasElseStatement(true);
		token = getNextToken();
		check("{", token);
		vector<shared_ptr<Node>> falseBody;
		parseBody(falseBody);
		node.setFalseBody(falseBody);
	}
	else resetReceivedToken();
	return node;
}

CycleStatementNode SyntaxAnalyzer::parseCycleStatement(const string& cycleKeyword)
{
	CycleStatementNode node;
	//parse condition
	vector<Token> condition;
	if (cycleKeyword == "for") {
		node.setFor(true);
		condition = parseForCondition();
	}
	else {
		node.setFor(false);
		condition = parseWhileCondition();
	}
	node.setCondition(condition);
	Token token = getNextToken();
	check("{", token);
	//TODO handle break/continue statements
	vector<shared_ptr<Node>> body;
	parseBody(body);
	node.setBody(body);
	return node;
}

vector<Token> SyntaxAnalyzer::parseForCondition()
{
	vector<Token> condition;
	Token token = getNextToken();
	check("(", token);
	token = getNextToken();
	condition.push_back(token);
	check("int", token);
	token = getNextToken();
	condition.push_back(token);
	checkIdentifier(token);
	const string iterator = token.getValue();
	//check var wasn't earlier
	token = getNextToken();
	condition.push_back(token);
	check("=", token);
	token = getNextToken();
	condition.push_back(token);
	if (token.getTokenType() != TokenTypesEnum::INT) {
		string message = "Expected integer value, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	condition.push_back(token);
	check(";", token);
	token = getNextToken();
	condition.push_back(token);
	if (token.getValue() != iterator) {
		string message = "Expected identifier '" + iterator + "', but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	condition.push_back(token);
	const string comparisonOperator = token.getValue();
	if (comparisonOperator != "<" && comparisonOperator != ">"
		&& comparisonOperator != "<=" && comparisonOperator != ">=") {
		string message = "Expected comparison operator, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	condition.push_back(token);
	if (token.getTokenType() != TokenTypesEnum::INT) {
		string message = "Expected integer value, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	condition.push_back(token);
	check(";", token);
	token = getNextToken();
	condition.push_back(token);
	if (token.getValue() != iterator) {
		string message = "Expected identifier '" + iterator + "', but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	condition.push_back(token);
	if (token.getValue() != "++" && token.getValue() != "--") {
		string message = "Expected increment or decrement, but was - " + token.getValue();
		cout << message;
		exit(1);
	}
	token = getNextToken();
	check(")", token);

	return condition;
}

vector<Token> SyntaxAnalyzer::parseWhileCondition()
{
	vector<Token> condition;
	Token token = getNextToken();
	check("(", token);
	token = getNextToken();
	while (token.getValue() != ")") {
		if (token.getTokenType() != TokenTypesEnum::OPERATOR && token.getTokenType() != TokenTypesEnum::BOOLEAN
			&& token.getTokenType() != TokenTypesEnum::CHAR && token.getTokenType() != TokenTypesEnum::FLOAT
			&& token.getTokenType() != TokenTypesEnum::INT && token.getTokenType() != TokenTypesEnum::STRING
			&& token.getTokenType() != TokenTypesEnum::IDENTIFIER && token.getValue() != "("
			&& token.getValue() != ")") {
			string message = "Value of unexpected type - " + token.getValue();
			cout << message;
			exit(1);
		}
		condition.push_back(token);
		token = getNextToken();
	}
	return condition;
}

ReturnStatementNode SyntaxAnalyzer::parseReturnStatement()
{
	Token token = getNextToken();
	vector<Token> returnResult;
	while (token.getValue() != ";") {
		returnResult.push_back(token);
		token = getNextToken();
	}
	ReturnStatementNode node;
	node.setResult(returnResult);
	return node;
}

bool SyntaxAnalyzer::checkTypeValueCompatibility(const string& dataType, const TokenTypesEnum& value)
{
	if (dataType == "String" && value == TokenTypesEnum::STRING
		|| dataType == "char" && value == TokenTypesEnum::CHAR
		|| dataType == "boolean" && value == TokenTypesEnum::BOOLEAN) {
		return true;
	}
	else if ((dataType == "int" || dataType == "long" || dataType == "short" || dataType == "byte")
		&& value == TokenTypesEnum::INT) {
		return true;
	}
	else if ((dataType == "float" || dataType == "double") && value == TokenTypesEnum::FLOAT) {
		return true;
	}
	else {
		false;
	}
}

void SyntaxAnalyzer::check(const string& expected, const Token& token)
{
	if (token.getValue() != expected) {
		string message = "Expected '" + expected + "', but was - " + token.getValue();
		cout << message;
		exit(1);
	}
}

void SyntaxAnalyzer::checkTokenForConditionOrExpression(const Token& token)
{
	if (token.getTokenType() != TokenTypesEnum::OPERATOR && token.getTokenType() != TokenTypesEnum::BOOLEAN
		&& token.getTokenType() != TokenTypesEnum::CHAR && token.getTokenType() != TokenTypesEnum::FLOAT
		&& token.getTokenType() != TokenTypesEnum::INT && token.getTokenType() != TokenTypesEnum::STRING
		&& token.getTokenType() != TokenTypesEnum::IDENTIFIER && token.getValue() != "("
		&& token.getValue() != ")") {
		string message = "Value of unexpected type - " + token.getValue();
		cout << message;
		exit(1);
	}
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

void SyntaxAnalyzer::checkUniqueClassAttribute(map<string, string>& earlierDeclaredVars, const string& identifier)
{
	if (earlierDeclaredVars.count(identifier)) {
		string message = "Class attribute '" + identifier + "' has already been declared";
		cout << message;
		exit(1);
	}
}

