#pragma once
#include <string>
#include <vector>
#include "Token.h"

const int typesCount = 8;

class DataTypesToken {
private:
	std::string type;
	std::vector <std::string> types;
	bool foundType;
	std::string awaitNextToken;
	std::string awaitLastToken;

public:
	DataTypesToken(Token anyToken);
	bool isTokenWasIdentified() const;
};