#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "Token.h"
#include "Translator.h"
#include "Node.h"
#include "SyntaxAnalyzer.h"

#include "TokenMapper.h"
#include "SemanticAnalyzer.h"
using namespace std;

int main() {
	setlocale(LC_ALL, "rus");

	//std::string line;
	//std::string text = "";
	////Чтение файла можно вынести в отдельный класс/метод
	//std::ifstream reader("put path here...");
	//if (reader.is_open()) {
	//	while (std::getline(reader, line)) {
	//		text += line;
	//	}
	//	reader.close();
	//	if (text.length() == 0) {
	//		std::cout << "File wasn't contain smth\n";
	//	}
	//}
	//else {
	//	std::cout << "Wrong path to file\n";
	//}
	Lexer lexer;
	Parser parser("javacode.txt");
	std::vector<Token> variable = lexer.extractTokens(parser.getText());
	cout << "Token vector:" << endl;
	cout << "----------------" << endl;
	for (int i = 0; i < variable.size(); i++) {
		std::cout << i << ": Type: " << variable.at(i).getTokenType() << "    Value: " << variable.at(i).getValue() << std::endl;
	}
	cout << "----------------" << endl;
	
	//Syntax analyzer
	SyntaxAnalyzer syntaxAnalyzer(variable);
	MainClassNode ast = syntaxAnalyzer.buildAst();
	cout << "AST tree:" << endl;
	cout << "----------------" << endl;
	ast.print();
	cout << "----------------" << endl;
	
	//Semantic analyzer here
	SemanticAnalyzer sa(variable);
	cout << "Semantic Analyzer prints:" << endl;
	cout << "----------------" << endl;
	sa.printAllVariables();
	cout << "----------------" << endl;
	sa.printAllExpressions(); 
	cout << "----------------" << endl;
	sa.doSemanticAnalysis();
	
	cout << "Pascal generated:" << endl;
	cout << "----------------" << endl;
	TokenMapper *tm = new TokenMapper(variable);
	tm->printPascalCode();
	return 0;
}