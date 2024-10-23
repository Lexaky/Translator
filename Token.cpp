#include "Token.h"

Token::Token(std::string text) {
	if (!text.empty())
		this->token = text;
	else {
		token = "";
		std::cout << "Пустой токен\n";
	}
}