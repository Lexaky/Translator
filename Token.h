#pragma once
#include <iostream>
#include <string>

class Token {
private:
	std::string token;
	// context is ([backContext], [token], [nextContext]), where back and next contexts is also tokens
	std::string backContext = "";
	std::string nextContext = "";

public:
	Token(std::string text);
	
	void getContext(); // Must return context in backContext and nextContext
	std::string getToken(); // return token
	

};