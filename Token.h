#pragma once
#include <iostream>
#include <string>

class Token {
private:
	std::string token;
	// Если при определении типа токена нужно знать контекст ([token-1] + [token] + [token+1])
	std::string context = "";

public:
	Token(std::string text);
	
	void getContext();
	// Далее методы-определители типа токена (что за слово или выражение)

};