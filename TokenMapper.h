#include <vector>
#include "Token.h"
#pragma once

class TokenMapper
{
private:
	std::map<std::string, std::string> JavaToPascalTokens = {
        {"abstract", "abstract"},
        {"assert", "assert"},
        {"boolean", "boolean"},
        {"break", "break"},
        {"byte", "byte"},
        {"case", "case"},
        {"catch", "except"},
        {"char", "char"},
        {"class", "class"},
        {"const", "const"},
        {"continue", "continue"},
        {"default", "else"},
        {"do", "do"},
        {"double", "double"},
        {"else", "else"},
        {"enum", "enum"},
        {"extends", "inherit"},
        {"final", "final"},
        {"finally", "finally"},
        {"float", "single"},
        {"for", "for"},
        {"goto", "goto"},
        {"if", "if"},
        {"implements", "implements"},
        {"import", "uses"},
        {"instanceof", "is"},
        {"int", "integer"},
        {"interface", "interface"},
        {"long", "int64"},
        {"native", "external"},
        {"new", "new"},
        {"null", "nil"},
        {"package", "unit"},
        {"private", "private"},
        {"protected", "protected"},
        {"public", "public"},
        {"return", "exit"},
        {"short", "smallint"},
        {"static", "static"},
        {"strictfp", "strictfp"},
        {"super", "inherited"},
        {"switch", "case"},
        {"synchronized", "synchronized"},
        {"this", "self"},
        {"throw", "raise"},
        {"throws", "raises"},
        {"transient", "transient"},
        {"try", "try"},
        {"void", "procedure"},
        {"volatile", "volatile"},
        {"while", "while"},
        // Символы и конструкции
        {"{", "begin"},
        {"}", "end"},
        {";", ";"},
        {"=", ":="},
        {"==", "="},
        {"!=", "<>"},
        {"&&", "and"},
        {"||", "or"},
        {"!", "not"},
        {"//", "//"},
        {"/**", "{**"},
        {"*/", "**}"},
        {"/*", "{*"},
        {"*/", "*}"},
        {"System.out.println", "writeln"},
        {"System.out.print", "write"},
        {"import", "uses"},
        {"public static void main", "begin"},
        {"\"", "\'"}
	};

    std::map<std::string, std::string> deleteTypes = {
        {"String", ""},
        {"char", ""},
        {"int", ""},
        {"boolean", ""},
        {"float", ""},
        {"double", ""},
        {"void", ""}
    };
	std::vector<Token> sourceTokens;
    std::vector <std::string> pascalTokens;
	void reconstructPascalTokens();
    void generateFunction(std::string functionName, std::string functionType, std::vector <std::string> varNames,
        std::vector<std::string> varTypes, std::vector<Token> tokens);
    void generateMethods(std::string methodName, std::vector<std::string> varNames,
        std::vector<std::string>, std::vector <Token> tokens);
    void globalGenerator(std::vector<Token> tokens);
    void ifGenerator(std::vector<Token> tokens);
    void whileGenerator(std::vector<Token> tokens);
    void elseGenerator(std::vector<Token> tokens);
    void doWhileGenerator(std::vector<Token> tokens);
    void forGenerator(std::vector<Token> tokens);
public:
	TokenMapper(std::vector<Token> tokens);
	std::vector<std::string> getPascalTokens();
	std::vector<Token> getJavaTokens();
	std::string operator [](int tokenid) {
		if (tokenid < sourceTokens.size() && tokenid >= 0)
			return sourceTokens.at(tokenid).getValue();
		else
			return "";
	}
	void setToken(Token anotherToken, int pos);
	void printPascalCode();
};

