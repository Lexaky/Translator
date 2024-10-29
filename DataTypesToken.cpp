#include "DataTypesToken.h"
#include "Token.h"
#include <vector>
#include <iostream>

DataTypesToken::DataTypesToken(Token anyToken) {
	types.reserve(typesCount);
	types.push_back("byte");
	types.push_back("short");
	types.push_back("int");
	types.push_back("long");
	types.push_back("float");
	types.push_back("double");
	types.push_back("char");
	types.push_back("boolean");

	foundType = false;

	for (int i = 0; i < types.size(); i++) {
		if (types.at(i) == anyToken.getToken()) {
			type = types.at(i);
			std::cout << "[DataTypesToken] found " << type << "\n";
			awaitNextToken = "<identificator>";
			awaitLastToken = ";";
			foundType = true;
		}
	}

	if (!foundType) {
		std::cout << "[DataTypesToken] Token wasn't found: " << anyToken.getToken() << "\n";
	}

}

bool DataTypesToken::isTokenWasIdentified() const {
	return foundType;
}