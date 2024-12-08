#include "Token.h"

Token::Token(std::string value, TokenTypesEnum tokenType)
{
	this->value = value;
	this->tokenType = tokenType;
}

std::string Token::getValue()
{
	return value;
}

TokenTypesEnum Token::getTokenType()
{
	return tokenType;
}
