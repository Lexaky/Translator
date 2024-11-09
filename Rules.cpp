#include "Rules.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool Derivation::tokenHaveOneDerivation() {
	return toToken.size() == 1;
}

void Derivation::setDefineRule() {
	if (definedRule == false) {
		definedRule = true;
	}
	else {
		std::cout << "[Derivation] Rule already have one derivation!\n'" << fromToken << 
			"' goes to '" << toTokenStr << "'\n";
		definedRule = true;
	}
}

Derivation::Derivation(std::string tokenFrom) {
	
}

void Derivation::pushToken(std::string toTokenPush) {

}

std::vector<std::string> Derivation::getTokenDerivation() {

}

std::vector<std::string> split(std::string str, std::string del) {
	std::vector<std::string> splittedString;
	std::string temp;
	bool flag;
	for (int i = 0; i < str.size(); i++) {
		temp = "";
		flag = true;
		for (int j = 0; j < del.size(); j++) {
			if (str.at(i) != del.at(j)) {
				flag = false;
				break;
			}
		}
		if (flag) {
			splittedString.push_back(temp);
			temp = "";
		}
		temp += str.at(i);
	}
	return splittedString;
}

Rules::Rules(std::string pathToFile) {
	std::ifstream file(pathToFile);
	std::vector <std::string> rules;
	if (!file.is_open()) {
		std::cout << "File with Rules wasn't opened!\n";
		return;
	}
	else {
		std::string line;
		//Writed all rules in vector rules
		while (std::getline(file, line)) {
			rules.push_back(line);
		}
		//Now it's tiMe tO PaRsInG
		this->rule.reserve(rules.size());
		for (int i = 0; i < rules.size(); i++) {
			VaultOfRules newRule;
			newRule.from = split(rules.at(i), "//").at(0);
			// the other part of splitted rules - is "to" rules
			
			rule.push_back(newRule);
			
		}
	}
}