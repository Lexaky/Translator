#include "Parser.h"

void Parser::removeSpaces() {
	text.erase(std::remove_if(text.begin(), text.end(), [](char c) {
		return c == ' ';
		}), text.end());
}

Parser::Parser(std::string pathToFile) {
	std::string line;
	this->text = "";
	std::ifstream reader(pathToFile);
	if (reader.is_open()) {
		while (std::getline(reader, line)) {
			this->text += line;
		}
		removeSpaces();
	}
	else {
		std::cout << "Неверный путь к файлу\n";
	}
}

std::string Parser::getText() {
	return text;
}

int Parser::countOf(std::string expression) {
	int sum = 0;
	bool flagOfContain;
	for (int i = 0; i < text.length() - expression.length() + 1; i++) {
		flagOfContain = true;
		for (int j = 0; j < expression.length(); j++) {
			if (text.at(i) != expression.at(j)) {
				flagOfContain = false;
				break;
			}
		}
		flagOfContain == true ? sum++ : sum = sum;
	}
	return sum;
}

int Parser::countOf(char expression) {
	int sum = 0;
	for (int i = 0; i < text.length(); i++) {
		if (text.at(i) == expression)
			sum++;
	}
	return sum;
}

bool Parser::isEmptyFile() {
	removeSpaces();
	return text.length() == 0;
}

std::vector<Token> Parser::execTokenization() {
	//Здесь должна быть логика разделения текста из файла на выражения
	std::vector <Token> words;
	words.reserve(text.length()); // as max
	
}