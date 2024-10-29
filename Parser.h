#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Token.h"

class Parser {
private:
	std::string text = "";

	void removeSpaces();

public:
	Parser(std::string pathToFile);

	std::string getText();
	int countOf(std::string expression); // Count of expression in text
	int countOf(char expression); //same
	bool isEmptyFile();
	std::vector <int> getPosListOf(char expr); // Vector of positions of expression in text
	std::vector <int> getPosListOf(std::string expr); //same
};