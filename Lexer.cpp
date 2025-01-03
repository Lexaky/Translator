#include "Lexer.h"

vector<Token> Lexer::extractTokens(string sourceCode) {
	string trimedCode = trim1(sourceCode);
	string tmp;
	string ch;
	int line = 1;
	int position = 0;
	size_t size = trimedCode.size();
	bool isStringNow = false;
	for (size_t i = 0; i < size; i++) {
		++position;
		ch = string(1, trimedCode.at(i));

		//String handling
		if (ch == "\"" && !isStringNow) {
			isStringNow = true;
			continue;
		} else if (ch == "\"" && isStringNow) {
			tokens.push_back(Token(tmp, TokenTypesEnum::STRING));
			tmp = "";
			isStringNow = false;
			continue;
		} else if (isStringNow) {
			if (i == size - 1) {
				string message = "Missing closing double quote in line " + to_string(line) + " position "
					+ to_string(position+1) + "\n";
				cout << message;
				exit(1);
			}
			if (ch == "\\" && i + 1 < size) {
				ch += string(1, trimedCode.at(++i));
			}
			tmp += ch;
			continue;
		}

		//Char handling
		if (ch == "'") {
			if (i + 2 < size && string(1, trimedCode.at(i + 2)) == "'") {
				tokens.push_back(Token(string(1, trimedCode.at(i + 1)), TokenTypesEnum::CHAR));
			} else {
				string message = "Missing closing quote in line " + to_string(line) + " position "
					+ to_string(position + 2) + "\n";
				cout << message;
				exit(1);
			}
			i += 2;
		}

		if (!AllowedSymbols::ALLOWED_SYMBOLS.count(ch)) {
			string message = "In line " + to_string(line) + " position " + to_string(position)
				+ " an invalid character was encountered - '" + ch + "'\n";
			cout << message;
			exit(1);
		}

		if (TokenTypes::IGNORE.count(ch)) {
			if (ch == "\n") {
				++line;
				position = 0;
			} else if (ch == "\t") {
				position += 4;
			}
			if (!tmp.empty()) {
				tokens.push_back(getToken(tmp));
				tmp = "";
			}
			continue;
		}

		if (TokenTypes::SPECIAL_SYMBOLS.count(ch)) {
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
				auto additionalCh = string(1, trimedCode.at(i+1));
				if (TokenTypes::OPERATORS.count(ch + additionalCh))
				{
					ch += additionalCh;
					++i;
				}
			}
			tokens.push_back(Token(ch, TokenTypesEnum::OPERATOR));
			continue;
		}

		tmp += ch;
	}
	string lastWord = trim2(tmp);
	if (!lastWord.empty()) {
		tokens.push_back(getToken(lastWord));
	}
	tokens.push_back(Token("", TokenTypesEnum::END_OF_FILE));
	return tokens;
}

string Lexer::trim1(const string& str) {
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	if (first == string::npos) {
		cout << "The text consists of nothing but spaces and space characters\n";
		exit(1);
	}

	size_t last = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(first, (last - first + 1));
}

string Lexer::trim2(const string& str) {
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	if (first == string::npos) {
		return str;
	}

	size_t last = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(first, (last - first + 1));
}

Token Lexer::getToken(string str)
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
	return Token(str, tryParseToken(str));
}

TokenTypesEnum Lexer::tryParseToken(string token) {
	if (token == "true" || token == "false") {
		return TokenTypesEnum::BOOLEAN;
	}
	if (canBeIdentifier(token)) {
		return TokenTypesEnum::IDENTIFIER;
	}

	TokenTypesEnum type = tryParseNumber(token);
	if (type == TokenTypesEnum::UNKNOWN) {
		string message = "Value '" + token + "' doesn't represent any Java constructions!";
		cout << message;
		exit(1);
	}
	else return type;
}

TokenTypesEnum Lexer::tryParseNumber(const string& str) {
	if (str.empty()) {
		return TokenTypesEnum::UNKNOWN;
	}

	bool hasDot = false;
	size_t i = 0;
	for (; i < str.length() - 1; ++i) {
		if (!std::isdigit(str[i])) {
			if (str[i] == '.') {
				if (hasDot) return TokenTypesEnum::UNKNOWN;
				hasDot = true;
			}
			else return TokenTypesEnum::UNKNOWN;
		}
	}

	if (!std::isdigit(str[i])) {
		return TokenTypesEnum::UNKNOWN;
	}

	if (hasDot) return TokenTypesEnum::FLOAT;
	else return TokenTypesEnum::INT;
}

bool Lexer::canBeIdentifier(const string& str) {
	if (str.empty()) return false;

	if (!std::isalpha(str[0])) return false;

	for (size_t i = 1; i < str.length(); ++i) {
		if (!std::isalnum(str[i])) return false;
	}

	return true;
}
