#pragma once
#include <string>
#include <vector>

struct Derivation {
	std::string fromToken;
	std::string toTokenStr;
	bool oneDerivationDefine = false; // Token goes only to one construction (<equeal operator> goes to "==" or "!=" etc.)
	bool definedRule = false; // Token goes only to string (For example <program> goes to "program")
	std::vector<std::string> toToken; // where token goes
	
	bool tokenHaveOneDerivation();
	void setDefineRule();
	Derivation(std::string tokenFrom);
	void pushToken(std::string toTokenPush);
	std::vector<std::string> getTokenDerivation();
};

struct VaultOfRules {
	std::string from;
	std::string to;
};

class Rules {
private:
	std::vector<VaultOfRules> rule;
	
public:
	Rules(std::string pathToFile);
	
};