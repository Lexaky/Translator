#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "Token.h"
#include "Translator.h"
#include "AST.h"
#include "ASTBuilder.h"

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
	for (int i = 0; i < variable.size(); i++) {
		std::cout << "Type: " << variable.at(i).getTokenType() << std::endl;
		std::cout << "Value: " << variable.at(i).getValue() << std::endl;
	}


	try {

		// Создание ASTBuilder и построение дерева
		ASTBuilder astBuilder(variable);
		std::shared_ptr<AST> ast = astBuilder.buildAST();

		// Вывод AST
		std::cout << "Abstract Syntax Tree:" << std::endl;
		ast->print(); // Метод print() в классе AST, который рекурсивно выводит дерево

	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}


	return 0;
}