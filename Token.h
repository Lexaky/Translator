#pragma once
#include <iostream>
#include <string>

#include "TokenTypesEnum.h"
#include "TokenTypes.h"

class Token {
private:
	std::string value;
	TokenTypesEnum tokenType;

public:
	Token(std::string value, TokenTypesEnum tokenType);
	std::string getValue();
	TokenTypesEnum getTokenType();
};