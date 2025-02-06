#include "Token.h"

Token::Token(std::string value, TokenTypesEnum tokenType)
{
	this->value = value;
	this->tokenType = tokenType;
}

std::string Token::getValue() const
{
	return value;
}

TokenTypesEnum Token::getTokenType() const
{
	return tokenType;
}
