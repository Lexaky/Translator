#pragma once
#include <iostream>
#include <string>

class Token {
private:
	std::string token;
	// ���� ��� ����������� ���� ������ ����� ����� �������� ([token-1] + [token] + [token+1])
	std::string context = "";

public:
	Token(std::string text);
	
	void getContext();
	// ����� ������-������������ ���� ������ (��� �� ����� ��� ���������)

};