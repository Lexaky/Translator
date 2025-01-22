#include "TokenMapper.h"
#include <string>
void TokenMapper::setToken(Token anotherToken, int pos) {
	if (pos > 0 && pos < sourceTokens.size())
		sourceTokens.at(pos) = anotherToken;
}

std::vector<std::string> TokenMapper::getPascalTokens() {
	return pascalTokens;
}
std::vector<Token> TokenMapper::getJavaTokens() {
	return sourceTokens;
}

TokenMapper::TokenMapper(std::vector<Token> tokens) {
	if (tokens.size() > 0) {
		sourceTokens = tokens;
		reconstructPascalTokens();
	}
	else {
		std::cout << "[TokenMapper] Java vector tokens is empty!\n";
	}
}

void TokenMapper::reconstructPascalTokens() {
    std::map<std::string, std::string> typeMapping = {
        {"String", "string"},
        {"char", "char"},
        {"int", "integer"},
        {"boolean", "boolean"},
        {"float", "real"},
        {"double", "double"},
        {"void", "void"}
    };
	std::vector<std::string> varNames;
	std::vector<std::string> varTypes;
    std::vector<std::string> area; //GLOBAL || function name params || function name local || procedure name params || procedure name local
    std::vector<std::string> functionName;
    std::vector<std::string> functionType;
    std::vector<std::string> methodName;
    
    bool isType = false;         // Флаг для типа данных
    std::string currentType;     // Текущий тип данных
    std::string currentArea = "GLOBAL"; // Текущая область видимости
    std::string currentFunctionOrMethod; // Текущая функция/метод

    for (size_t i = 0; i < sourceTokens.size(); ++i) {
        const Token& token = sourceTokens[i];

        // Если токен — тип данных или void
        if (typeMapping.find(token.getValue()) != typeMapping.end()) {
            isType = true;
            currentType = typeMapping[token.getValue()];
        }
        // Если токен — идентификатор и мы обрабатываем переменную
        else if (isType && token.getTokenType() == IDENTIFIER) {
            const std::string& identifier = token.getValue();

            // Проверяем, является ли следующий токен "(" — это функция или метод
            if (i + 1 < sourceTokens.size() && sourceTokens[i + 1].getValue() == "(") {
                if (currentType == "void") {
                    // Это процедура
                    methodName.push_back(identifier);
                    currentArea = identifier;
                }
                else {
                    // Это функция
                    functionName.push_back(identifier);
                    functionType.push_back(currentType);
                    currentArea = identifier;
                }
                currentFunctionOrMethod = identifier;
                ++i; // Пропускаем "("

                // Обрабатываем параметры функции или метода
                while (i + 1 < sourceTokens.size() && sourceTokens[++i].getValue() != ")") {
                    if (typeMapping.find(sourceTokens[i].getValue()) != typeMapping.end()) {
                        std::string paramType = typeMapping[sourceTokens[i].getValue()];
                        if (i + 1 < sourceTokens.size() && sourceTokens[i + 1].getTokenType() == IDENTIFIER) {
                            varNames.push_back(sourceTokens[++i].getValue());
                            varTypes.push_back(paramType + "_");
                            area.push_back(currentFunctionOrMethod);
                        }
                    }
                }
            }
            else {
                // Это обычная переменная
                varNames.push_back(identifier);
                varTypes.push_back(currentType + "");
                area.push_back(currentArea);
            }

            isType = false; // Завершаем обработку текущего типа
        }
        // Если токен — "{" — начинаем блок (сбрасываем область)
        else if (token.getValue() == "{") {
            // Продолжаем текущую область видимости
        }
        // Если токен — "}" — заканчиваем блок
        else if (token.getValue() == "}") {
            currentArea = "GLOBAL"; // Возвращаемся к глобальной области
            currentFunctionOrMethod.clear();
        }

        // Обновляем текущую область видимости, если находимся внутри функции или метода
        if (!currentFunctionOrMethod.empty()) {
            currentArea = currentFunctionOrMethod;
        }
    }

    // Извлёк все названия переменных и их типы данных уже в Pascal-евском виде
    // Извлёк названия функций и их типов, методов. Для переменных видно, к какому методу/функции они принадлежат
    std::cout << "Program Main;\n";
    for (int i = 0; i < varNames.size(); i++) {
        if (area.at(i) == "GLOBAL" || area.at(i) == "main")
            std::cout << "var " << varNames.at(i) + " : " + varTypes.at(i) + ";\n";
    }
    std::vector<std::string> varNamesInFunction;
    varNamesInFunction.reserve(varNames.size());
    std::vector<std::string> varTypesInFunction;
    varTypesInFunction.reserve(varTypes.size());
    std::vector<Token> bodyFunction;
    bodyFunction.reserve(sourceTokens.size());
    // Далее генерация функций пользователя:
    for (int i = 0; i < functionName.size(); i++) {
        if (functionName.at(i) == "main")
            continue;

        for (int j = 0; j < sourceTokens.size(); j++) {
            if (sourceTokens.at(j).getValue() == functionName.at(i)) {
                while (sourceTokens.at(j).getValue() != "{")
                    j++;
                j++;
                int nestedLevel = 0;
                while (nestedLevel != 0 || sourceTokens.at(j).getValue() != "}") {
                    bodyFunction.push_back(sourceTokens.at(j));
                    if (sourceTokens.at(j).getValue() == "{") {
                        nestedLevel++;
                    }
                    j++;
                    if (sourceTokens.at(j).getValue() == "}" && nestedLevel > 0) {
                        nestedLevel--;
                    }
                }

                for (int k = 0; k < area.size(); k++) {
                    if (area.at(k) == functionName.at(i)) {
                        varNamesInFunction.push_back(varNames.at(k));
                        varTypesInFunction.push_back(varTypes.at(k));
                    }
                }
                generateFunction(functionName.at(i), functionType.at(i), varNamesInFunction, varTypesInFunction, bodyFunction);
                bodyFunction.clear();
                varNamesInFunction.clear();
                varTypesInFunction.clear();
            }

        }
    }
    bodyFunction.clear();
    varNamesInFunction.clear();
    varTypesInFunction.clear();
    // Генерация методов пользователя:
    for (int i = 0; i < methodName.size(); i++) {
        for (int j = 0; j < sourceTokens.size(); j++) {
            if (sourceTokens.at(j).getValue() == methodName.at(i)) {
                while (sourceTokens.at(j).getValue() != "{")
                    j++;
                j++;
                int nestedLevel = 0;
                while (nestedLevel != 0 || sourceTokens.at(j).getValue() != "}") {
                    bodyFunction.push_back(sourceTokens.at(j));
                    if (sourceTokens.at(j).getValue() == "{") {
                        nestedLevel++;
                    }
                    j++;
                    if (sourceTokens.at(j).getValue() == "}" && nestedLevel > 0) {
                        nestedLevel--;
                        bodyFunction.push_back(sourceTokens.at(j));
                        j++;
                    }
                }
                for (int k = 0; k < area.size(); k++) {
                    if (area.at(k) == methodName.at(i)) {
                        varNamesInFunction.push_back(varNames.at(k));
                        varTypesInFunction.push_back(varTypes.at(k));
                    }
                }
                generateMethods(methodName.at(i), varNamesInFunction, varTypesInFunction, bodyFunction);
                bodyFunction.clear();
                varNamesInFunction.clear();
                varTypesInFunction.clear();
            }
            
        }
    }
}

void TokenMapper::generateFunction(std::string functionName, std::string functionType, std::vector <std::string> varNames,
    std::vector<std::string> varTypes, std::vector<Token> tokens) {
    std::cout << "function " + functionName + "(";
    for (int i = 0; i < varNames.size(); i++) {
        if (varTypes.at(i).at(varTypes.at(i).size() - 1) == '_')
        {
            std::cout << varNames.at(i) << " : " << varTypes.at(i);
            if (i + 1 != varNames.size())
                std::cout << ", ";
        }
    }
    std::cout << ") : " << functionType << ";\n";
    for (int i = 0; i < varNames.size(); i++) {
        if (varTypes.at(i).at(varTypes.at(i).size() - 1) != '_')
        {
            std::cout << "var " << varNames.at(i) << " : " << varTypes.at(i) << ";\n";
        }
    }
    std::cout << "begin\n";

    for (int i = 0; i < tokens.size(); i++) {
        //Что-то придумать с токеном "return", он должен заменяться на [functionName] := 
    }
    
    std::cout << "end;\n";
}

void TokenMapper::generateMethods(std::string methodName, std::vector<std::string> varNames,
    std::vector<std::string> varTypes, std::vector <Token> tokens) {
    if (methodName == "main") {
        std::cout << "begin\n";
        globalGenerator(tokens);
        std::cout << "end.\n";
        return;
    }
    std::cout << "procedure " + methodName + "(";
    for (int i = 0; i < varNames.size(); i++) {
        if (varTypes.at(i).at(varTypes.at(i).size() - 1) == '_')
        {
            std::cout << varNames.at(i) << " : " << varTypes.at(i).substr(0, varTypes.at(i).size()-1);
            if (i + 1 != varNames.size()-1)
                std::cout << ", ";
        }
    }
    std::cout << ")" << ";\n";
    for (int i = 0; i < varNames.size(); i++) {
        if (varTypes.at(i).at(varTypes.at(i).size() - 1) != '_')
        {
            std::cout << "var " << varNames.at(i) << " : " << varTypes.at(i) << ";\n";
        }
    }
    std::cout << "begin\n";

    globalGenerator(tokens);

    std::cout << "end;\n";
}


void TokenMapper::globalGenerator(std::vector<Token> tokens) {
    //tokens содержит внутренние конструкции методов (включая метод main)
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens.at(i).getValue() == "if") {
            //Создать массив токенов всего if'а, включая конец }
        }
        else if (tokens.at(i).getValue() == "while") {

        }
        else if (tokens.at(i).getValue() == "else") {

        }
        else if (tokens.at(i).getValue() == "do") {

        }
        else if (tokens.at(i).getValue() == "for") {

        }
        auto it = deleteTypes.find(tokens.at(i).getValue());
        if (it == deleteTypes.end()) {
            auto it1 = JavaToPascalTokens.find(tokens.at(i).getValue());
            if (it1 != JavaToPascalTokens.end()) {
                std::cout << JavaToPascalTokens[tokens.at(i).getValue()] << " ";
            }
            else {
                std::cout << tokens.at(i).getValue() << " ";
            }
        }
    }
}

void TokenMapper::ifGenerator(std::vector<Token> tokens) {
    std::cout << "if ";
    int i = 0;
    if (tokens.at(i).getValue() == "if")
        i++;
    std::vector<Token> ifExpression;
    ifExpression.reserve(sourceTokens.size());
    
    if (tokens.at(i).getValue() == "(")
        i++;
    while (tokens.at(i).getValue() != ")") {
        ifExpression.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(ifExpression);
    std::cout << "then begin\n";
    std::vector<Token> insideIf;
    insideIf.reserve(sourceTokens.size());
    int nestedLevel = 0;
    while (nestedLevel != 0 || tokens.at(i).getValue() != "}") {
        if (tokens.at(i).getValue() == "}") {
            nestedLevel--;
        }
        if (tokens.at(i).getValue() == "{") {
            nestedLevel++;
        }
        insideIf.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideIf);
    std::cout << "end\n";
}

void TokenMapper::whileGenerator(std::vector<Token> tokens) {
    std::cout << "while ";
    int i = 0;
    if (tokens.at(i).getValue() == "while")
        i++;
    std::vector<Token> ifExpression;
    ifExpression.reserve(sourceTokens.size());

    if (tokens.at(i).getValue() == "(")
        i++;
    while (tokens.at(i).getValue() != ")") {
        ifExpression.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(ifExpression);
    std::cout << "do begin\n";
    std::vector<Token> insideIf;
    insideIf.reserve(sourceTokens.size());
    int nestedLevel = 0;
    while (nestedLevel != 0 || tokens.at(i).getValue() != "}") {
        if (tokens.at(i).getValue() == "}") {
            nestedLevel--;
        }
        if (tokens.at(i).getValue() == "{") {
            nestedLevel++;
        }
        insideIf.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideIf);
    std::cout << "end\n";
}

void TokenMapper::elseGenerator(std::vector<Token> tokens) {
    std::cout << "else begin\n";
    int i = 0;
    std::vector<Token> insideElse;
    insideElse.reserve(sourceTokens.size());
    int nestedLevel = 0;
    while (nestedLevel != 0 || tokens.at(i).getValue() != "}") {
        if (tokens.at(i).getValue() == "}") {
            nestedLevel--;
        }
        if (tokens.at(i).getValue() == "{") {
            nestedLevel++;
        }
        insideElse.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideElse);
    std::cout << "end\n";
}

void TokenMapper::doWhileGenerator(std::vector<Token> tokens) {
    std::cout << "repeat\n";
    int i = 0;
    std::vector<Token> insideRepeat;
    insideRepeat.reserve(sourceTokens.size());
    if (tokens.at(i).getValue() == "{")
        i++;
    else if (tokens.at(i + 1).getValue() == "{")
        i+2;
    while (tokens.at(i).getValue() != "while") {
        if (tokens.at(i).getValue() == "}" && tokens.at(i + 1).getValue() == "while")
            break;
        else
            insideRepeat.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideRepeat);
    std::vector<Token> insideWhile;
    insideWhile.reserve(sourceTokens.size());
    std::cout << " until ";
    while (tokens.at(i).getValue() != ";") {
        insideWhile.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideWhile);
    std::cout << ";\n";
}

void TokenMapper::forGenerator(std::vector<Token> tokens) {

}

void TokenMapper::printPascalCode() {
	for (int i = 0; i < pascalTokens.size(); i++) {
		std::cout << pascalTokens.at(i) << " ";
	}
}