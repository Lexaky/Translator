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
	Token():value("0"),tokenType(UNKNOWN) {}
	std::string getValue() const;
	TokenTypesEnum getTokenType() const;
};