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
    std::vector<std::string> varGlobalStartValue;
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
                if (sourceTokens[i + 1].getValue() == "=" && sourceTokens[i + 1].getTokenType() == OPERATOR && currentArea == "GLOBAL") {
                    varGlobalStartValue.push_back(sourceTokens[i + 2].getValue());
                }
                else if (currentArea == "GLOBAL") {
                    varGlobalStartValue.push_back("_hasNotInit__");
                }
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
    for (int i = 0, j = 0; i < varNames.size(); i++) {
        if (area.at(i) == "GLOBAL" || area.at(i) == "main")
        {
            std::cout << "var " << varNames.at(i) + " : " + varTypes.at(i);

            if (varGlobalStartValue.size() > 0 && varGlobalStartValue.at(j) != "_hasNotInit__" && area.at(i) == "GLOBAL") {
                if (varTypes.at(i) == "string" || varTypes.at(i) == "char")
                {
                    varGlobalStartValue.at(j) = "\'" + varGlobalStartValue.at(j) + "\'";
                }
                std::cout << " := " << varGlobalStartValue.at(j);
                j++;
            }

            std::cout << ";\n";
        }
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
    bodyFunction.reserve(sourceTokens.size());
    // Генерация методов пользователя:
    for (int i = 0; i < methodName.size(); i++) {
        for (int j = 0; j < sourceTokens.size(); j++) {
            if (sourceTokens.at(j).getValue() == methodName.at(i)) {
                while (sourceTokens.at(j).getValue() != "{")
                    j++;
                j++;
                int nestedLevel = 0;
                while (nestedLevel != 0 || sourceTokens.at(j).getValue() != "}") {
                    if (j < sourceTokens.size() && sourceTokens.at(j).getValue() == "}" && nestedLevel > 0) {
                        nestedLevel--;
                    }
                    
                    if (j < sourceTokens.size() && sourceTokens.at(j).getValue() == "{") {
                        nestedLevel++;
                    }
                    if (j < sourceTokens.size())
                        bodyFunction.push_back(sourceTokens.at(j));
                    j++;
                    
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
                std::cout << "; ";
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
    std::vector <Token> newTokens;
    newTokens.reserve(tokens.size() + 1);
    for (int i = 0; i < tokens.size(); i++) {
        //Что-то придумать с токеном "return", он должен заменяться на [functionName] := 
        if (tokens.at(i).getValue() == "return" && tokens.at(i).getTokenType() == KEY_WORD) {
            Token returnToken(functionName, KEY_WORD);
            newTokens.push_back(returnToken);
            Token equalToken("=", OPERATOR);
            newTokens.push_back(equalToken);
            i++;
        }
        else {
            newTokens.push_back(tokens.at(i));
        }
    }
    globalGenerator(newTokens);
    
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
                std::cout << "; ";
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
            std::vector<Token> ifTokens;
            ifTokens.reserve(tokens.size());
            int j = i, nestedLevel = 0;
            while (tokens.at(j).getValue() != "}" || nestedLevel != 0) {
                ifTokens.push_back(tokens.at(j));
                j++;
                if (tokens.at(j).getValue() == "}" && nestedLevel > 0) {
                    nestedLevel--;
                }
                if (tokens.at(j).getValue() == "{") {
                    nestedLevel++;
                }
            }
            ifTokens.push_back(tokens.at(j));
            ifGenerator(ifTokens);
            i = j + 1;
            if (tokens.at(i).getValue() != "else") {
                std::cout << ";";
            }
            std::cout << "\n";
            ifTokens.clear();
            if (tokens.at(i).getValue() == "else") {
                ifTokens.reserve(tokens.size());
                int j = i, nestedLevel = 0;
                while (tokens.at(j).getValue() != "}" || nestedLevel != 0) {
                    ifTokens.push_back(tokens.at(j));
                    j++;
                    if (tokens.at(j).getValue() == "}" && nestedLevel > 0) {
                        nestedLevel--;
                    }
                    if (tokens.at(j).getValue() == "{") {
                        nestedLevel++;
                    }
                }
                ifTokens.push_back(tokens.at(j));
                elseGenerator(ifTokens);
                i = j + 1;
                continue;
                std::cout << "\n";
            }
        }
        else if (tokens.at(i).getValue() == "while") {
                std::vector<Token> ifTokens;
                ifTokens.reserve(tokens.size());
                int j = i, nestedLevel = 0;
                while (tokens.at(j).getValue() != "}" || nestedLevel != 0) {
                    ifTokens.push_back(tokens.at(j));
                    j++;
                    if (tokens.at(j).getValue() == "}" && nestedLevel > 0) {
                        nestedLevel--;
                    }
                    if (tokens.at(j).getValue() == "{") {
                        nestedLevel++;
                    }
                }
                ifTokens.push_back(tokens.at(j));
                whileGenerator(ifTokens);
                i = j;
                std::cout << "\n";
                continue;
        }
        else if (tokens.at(i).getValue() == "for") {
            std::vector<Token> forTokens;
            forTokens.reserve(tokens.size());
            int j = i, nestedLevel = 0;
            while (tokens.at(j).getValue() != "}" || nestedLevel != 0) {
                forTokens.push_back(tokens.at(j));
                j++;
                if (tokens.at(j).getValue() == "}" && nestedLevel > 0) {
                    nestedLevel--;
                }
                if (tokens.at(j).getValue() == "{") {
                    nestedLevel++;
                }
            }
            forTokens.push_back(tokens.at(j));
            forGenerator(forTokens);
            i = j;
            std::cout << "\n";
            continue;
        }
        auto it = deleteTypes.find(tokens.at(i).getValue());
        if (it == deleteTypes.end()) {
            auto it1 = JavaToPascalTokens.find(tokens.at(i).getValue());
            if (it1 != JavaToPascalTokens.end() && (tokens.at(i).getTokenType() < 5 || tokens.at(i).getTokenType() > 9)) {
                std::cout << JavaToPascalTokens[tokens.at(i).getValue()] << " ";
            }
            else {
                if (tokens.at(i).getTokenType() == STRING || tokens.at(i).getTokenType() == CHAR)
                    std::cout << "\'" << tokens.at(i).getValue() << "\'";
                else
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
    i += 2;
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
    std::cout << "end";
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
    i++;
    globalGenerator(ifExpression);
    std::cout << "do begin\n";
    std::vector<Token> insideIf;
    insideIf.reserve(sourceTokens.size());
    int nestedLevel = 0;
    i++;
    while (nestedLevel != 0 || tokens.at(i).getValue() != "}") {
        if (tokens.at(i).getValue() == "}" && nestedLevel > 0) {
            nestedLevel--;
        }
        if (tokens.at(i).getValue() == "{") {
            nestedLevel++;
        }
        insideIf.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideIf);
    std::cout << "end;\n";
}

void TokenMapper::elseGenerator(std::vector<Token> tokens) {
    std::cout << "else begin\n";
    int i = 2;
    std::vector<Token> insideElse;
    insideElse.reserve(sourceTokens.size());
    int nestedLevel = 0;
    while (nestedLevel != 0 || tokens.at(i).getValue() != "}") {
        if (tokens.at(i).getValue() == "}" && nestedLevel > 0) {
            nestedLevel--;
        }
        if (tokens.at(i).getValue() == "{") {
            nestedLevel++;
        }
        insideElse.push_back(tokens.at(i));
        i++;
    }
    globalGenerator(insideElse);
    std::cout << "end;\n";
}

void TokenMapper::forGenerator(std::vector<Token> tokens) {
    std::cout << "for ";
    int i = 1;
    std::vector <Token> forInit;
    forInit.reserve(tokens.size());
    while (tokens.at(i).getValue() != "(")
        i++;
    i++;
    while (tokens.at(i).getValue() != ")")
    {
        forInit.push_back(tokens.at(i));
        i++;
    }
    std::vector <Token> firstPart; firstPart.reserve(forInit.size());
    std::vector <Token> secondPart; secondPart.reserve(forInit.size());
    std::vector <Token> thirdPart; thirdPart.reserve(forInit.size());
    int j = 0;
    while (forInit.at(j).getValue() != ";") {
        firstPart.push_back(forInit.at(j));
        j++;
    }
    j++;
    while (forInit.at(j).getValue() != ";") {
        secondPart.push_back(forInit.at(j));
        j++;
    }
    j++;
    while (j != forInit.size()) {
        thirdPart.push_back(forInit.at(j));
        j++;
    }
    j += 2;
    // for (firstPart; secondPart; thirdPart) { ... }

    std::vector<Token> insideFor;
    insideFor.reserve(tokens.size());
    
    bool flagNegative = false;
    bool flagPositive = false;

    globalGenerator(firstPart);
    

    std::string leftIdentifierName;
    bool isIdentifierFound = false;

    for (int k = 0; k < firstPart.size(); k++) {
        if (firstPart.at(k).getTokenType() == IDENTIFIER) {
            leftIdentifierName = firstPart.at(k).getValue();
            isIdentifierFound = true;
            break;
        }
    }
    
    if (isIdentifierFound == false) {
        leftIdentifierName = "i";
    }

    for (int k = secondPart.size() - 1; k >= 0; k--) {
        if (secondPart.at(k).getTokenType() == OPERATOR) {
            if (secondPart.at(k).getValue() == leftIdentifierName &&
                (secondPart.at(k).getValue() == "<" ||
                secondPart.at(k).getValue() == "<=" ||
                secondPart.at(k).getValue() == "!=" ||
                secondPart.at(k).getValue() == "==")) {
                flagPositive = true;
                break;
            }
            else if ()
            else {
                flagNegative = true;
                break;
            }
        }
    }

    if (flagPositive == true) {
        std::cout << " to ";
    }
    else {
        std::cout << " downto ";
    }

    for (int k = 0; k < secondPart.size(); k++) {
        if (secondPart.at(k).getTokenType() == IDENTIFIER && secondPart.at(k).getValue() != leftIdentifierName) {
            std::cout << secondPart.at(k).getValue() << " ";
        }
        else if (secondPart.at(k).getTokenType() == INT || secondPart.at(k).getTokenType() == FLOAT) {

        }
        else {
            //string or char

        }
    }
    std::cout << "do begin\n";

    //forInside here

    std::cout << "end;";
}

void TokenMapper::printPascalCode() {
	for (int i = 0; i < pascalTokens.size(); i++) {
		std::cout << pascalTokens.at(i) << " ";
	}
}