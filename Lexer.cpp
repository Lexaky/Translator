#include "Lexer.h"

std::vector<Token> Lexer::extractTokens(std::string sourceCode)
{
	std::string trimedCode = trim(sourceCode);
	std::string tmp;
	std::string ch;
	size_t size = trimedCode.size();
	for (size_t i = 0; i < size; i++)
	{
		ch = std::string(1, trimedCode.at(i));
		if (TokenTypes::IGNORE.count(ch))
		{
			if (!tmp.empty()) {
				tokens.push_back(getToken(tmp));
				tmp = "";
			}
			continue;
		}
		if (TokenTypes::SPECIAL_SYMBOLS.count(ch))
		{
			if (!tmp.empty()) {
				tokens.push_back(getToken(tmp));
				tmp = "";
			}
			tokens.push_back(Token(ch, TokenTypesEnum::SPECIAL_SYMBOL));
			continue;
		}
		if (TokenTypes::OPERATORS.count(ch))
		{
			if (!tmp.empty()) {
				tokens.push_back(getToken(tmp));
				tmp = "";
			}
			if (i + 1 < size)
			{
				auto additionalCh = std::string(1, trimedCode.at(i));
				if (TokenTypes::OPERATORS.count(ch + additionalCh))
				{
					ch += additionalCh;
					i++;
				}
			}
			tokens.push_back(Token(ch, TokenTypesEnum::OPERATOR));
		}

		tmp += ch;
	}
	return tokens;
}

std::string Lexer::trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	if (first == std::string::npos) {
		throw std::exception("Текст стоит из одних пробелов и пробельных символов");		//можно будет сделать кастомные исключения
	}

	size_t last = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(first, (last - first + 1));
}

Token Lexer::getToken(std::string str)
{
	if (TokenTypes::ACCESS_MODIFIERS.count(str))
	{
		return Token(str, TokenTypesEnum::ACCESS_MODIFIER);
	}
	if (TokenTypes::KEY_WORDS.count(str))
	{
		return Token(str, TokenTypesEnum::KEY_WORD);
	}
	if (TokenTypes::DATA_TYPES.count(str)) {
		return Token(str, TokenTypesEnum::DATA_TYPE);
	}
	if (TokenTypes::OPERATORS.count(str)) {
		return Token(str, TokenTypesEnum::OPERATOR);
	}
	return Token(str, TokenTypesEnum::UNKNOWN);
}
