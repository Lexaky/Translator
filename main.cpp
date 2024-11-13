#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "Token.h"
#include "Translator.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");

	//std::string line;
	//std::string text = "";
	////Чтение файла можно вынести в отдельный класс/метод
	//std::ifstream reader("put path here...");
	//if (reader.is_open()) {
	//	while (std::getline(reader, line)) {
	//		text += line;
	//	}
	//	reader.close();
	//	if (text.length() == 0) {
	//		std::cout << "File wasn't contain smth\n";
	//	}
	//}
	//else {
	//	std::cout << "Wrong path to file\n";
	//}
	//Translator translator;
	//std::string result = translator.translate(text);
	//std::cout << result;

	return 0;
}