#include "SemanticAnalyzer.h"


SemanticAnalyzer::SemanticAnalyzer(std::vector<Token> toks) {
	if (toks.size() > 0) {
		tokens = toks;
		fillAllVariables();
        getAllExpressionsByVariablesAndReturn();
	}
	else {
		std::cout << "Empty tokens list\n";
	}
}

void SemanticAnalyzer::fillAllVariables() {
    std::map<std::string, std::string> typeMapping = {
        {"String", "string"},
        {"char", "char"},
        {"int", "integer"},
        {"boolean", "boolean"},
        {"float", "real"},
        {"double", "double"},
        {"void", "void"}
    };
    allVariables.reserve(tokens.size());
    bool isType = false;         // data type flag
    std::string currentType;     // current data type
    std::string currentArea = "GLOBAL"; // current vision area
    std::string currentFunctionOrMethod; // current func\method
    varsInProgram objVIP;
    int bracketscounter = -1;
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];

        // if token - data type or void
        if (typeMapping.find(token.getValue()) != typeMapping.end()) {
            isType = true;
            currentType = typeMapping[token.getValue()];
        }
        // if token - identifier and we proccess variable
        else if (isType && token.getTokenType() == IDENTIFIER) {
            const std::string& identifier = token.getValue();

            // check, is next token "(" - func or void
            if (i + 1 < tokens.size() && tokens[i + 1].getValue() == "(") {
    //            if (currentType == "void") {
    //                // is void
    //                objVIP.varName = identifier;
    //                objVIP.area = identifier;
    //            }
    //            else {
                    // is func
                    objVIP.varName = identifier;
                    objVIP.varType = currentType;
                    objVIP.area = identifier; // added myself
                    objVIP.inParameter = false; // added myself
                    allVariables.push_back(objVIP); // added myself
                    currentArea = identifier;
     //           }
                currentFunctionOrMethod = identifier;
                ++i; // skip "("

                // process parametres of func or void
                while (i + 1 < tokens.size() && tokens[++i].getValue() != ")") {
                    if (typeMapping.find(tokens[i].getValue()) != typeMapping.end()) {
                        std::string paramType = typeMapping[tokens[i].getValue()];
                        if (i + 1 < tokens.size() && tokens[i + 1].getTokenType() == IDENTIFIER) {
                            objVIP.varName = tokens[++i].getValue();
                            objVIP.varType = paramType;
                            objVIP.inParameter = true;
                            objVIP.area = currentFunctionOrMethod;
                            allVariables.push_back(objVIP);
                        }
                    }
                }
            }
            else {
                // simple variable
                objVIP.varName = identifier;
                objVIP.varType = currentType;
                objVIP.area = currentArea;
                objVIP.inParameter = false;
                allVariables.push_back(objVIP);
            }

            isType = false; // end processing of current type
        }
        // add counter of brackets if "{"
        else if (token.getValue() == "{" && token.getTokenType() == SPECIAL_SYMBOL) {
            bracketscounter++;
        }
        // dif counter of brackets if "}" or reset area if counter hit 0
        else if (token.getValue() == "}" && token.getTokenType() == SPECIAL_SYMBOL) {
            bracketscounter--;
            if (bracketscounter == 0) {
                currentArea = "GLOBAL"; // return to global area
                currentFunctionOrMethod.clear();
            }
        }

        // update current vision area if we are inside of func or void (just in case)
        if (!currentFunctionOrMethod.empty()) {
            currentArea = currentFunctionOrMethod;
        }
    }
}

void SemanticAnalyzer::printAllVariables() {
    for (int i = 0; i < allVariables.size(); i++) {
        std::cout << allVariables.at(i).varType << ": " << allVariables.at(i).varName << " in " <<
            allVariables.at(i).area << "; parameter? " << allVariables.at(i).inParameter << "\n";
    }
}

void SemanticAnalyzer::getAllExpressionsByVariablesAndReturn() {
    eip.reserve(tokens.size());
    expressionsInProgram eipObj;
    eipObj.expr.reserve(tokens.size());

    std::string currentArea = "GLOBAL";  // Default vision area
    std::vector<std::string> areaStack;  // Stack for nested areas

    std::vector<std::string> ignorelist = {"while", "if", "else", "for", "switch", "case"};

    int i = 0;
    int bracketscounter = 0;
    while (i < tokens.size()) {
        const Token& currentToken = tokens.at(i);
        if (currentToken.getValue() == "Main") {
            i++;
            continue;
        }

        // Handle method declaration and capture its parameters
        if (currentToken.getTokenType() == IDENTIFIER && i + 1 < tokens.size() && tokens.at(i + 1).getValue() == "(") {
            // add method modifiers (public, static, etc.) if it is a declaration + method name
            int j = i - 1;
            bool isdeclarationflag = false;
            while (tokens.at(j).getTokenType() != SPECIAL_SYMBOL) {
                j--;
                isdeclarationflag = true;
            }
            while (j++ < i) { eipObj.expr.push_back(tokens.at(j)); }
            
            eipObj.expr.push_back(tokens.at(i + 1)); // Add "("
            // process parametres of method
            int paramCounter;
            if (isdeclarationflag) {
                paramCounter = 1;
            }
            else { paramCounter = 2; }
            
            j = i + 2;

            // Process until the closing parenthesis of the parameter list
            while (j < tokens.size() && paramCounter > 0) {
                eipObj.expr.push_back(tokens.at(j));
                if (tokens.at(j).getValue() == "(") paramCounter++;
                if (tokens.at(j).getValue() == ")") paramCounter--;
                j++;
            }

            if (!isdeclarationflag) { eipObj.expr.pop_back(); }

            eipObj.area = currentArea;
            // Check if the next token indicates a method body
            if (j < tokens.size() && (tokens.at(j).getValue() == "{" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL)) {
                currentArea = currentToken.getValue();  // Set the method area
                areaStack.push_back(currentArea);
                bracketscounter = 1;
            }

            eip.push_back(eipObj);
            eipObj.expr.clear();
            i = j + 1;
            continue;
        }

        // add counter of brackets if "{"
        if (currentToken.getValue() == "{" && currentToken.getTokenType() == SPECIAL_SYMBOL) {
            bracketscounter++;
        }

        // dif counter of brackets if "}" or pop_back area if counter hit 0
        if (currentToken.getValue() == "}" && currentToken.getTokenType() == SPECIAL_SYMBOL) {
            bracketscounter--;
            if (bracketscounter == 0) {
                if (!areaStack.empty()) {
                    areaStack.pop_back();
                }
                currentArea = areaStack.empty() ? "GLOBAL" : areaStack.back();
            }
        }

        // Capture expressions involving identifiers or return statements
        if (currentToken.getTokenType() == IDENTIFIER || currentToken.getValue() == "return") {
            int backMovementer = i, forwardMovementer = i;

            // Move backward to capture the beginning of the expression
            while (backMovementer > 0 && (tokens.at(backMovementer).getValue() == "[" || tokens.at(backMovementer).getValue() == "]" ||
                tokens.at(backMovementer).getValue() == "(" || tokens.at(backMovementer).getValue() == ")" ||
                tokens.at(backMovementer).getTokenType() != SPECIAL_SYMBOL)) {
                backMovementer--;
            }

            // Move forward to capture the end of the expression
            while (forwardMovementer < tokens.size() && (tokens.at(forwardMovementer).getValue() == "[" || tokens.at(forwardMovementer).getValue() == "]" ||
                tokens.at(forwardMovementer).getValue() == "(" || tokens.at(forwardMovementer).getValue() == ")" ||
                tokens.at(forwardMovementer).getTokenType() != SPECIAL_SYMBOL)) {
                forwardMovementer++;
            }

            // Collect tokens for the current expression
            if (backMovementer == 0) {
                for (int j = backMovementer; j < forwardMovementer; j++) {
                    auto k = std::find(ignorelist.begin(), ignorelist.end(), tokens.at(j).getValue());
                    if (k == ignorelist.end()) {
                        eipObj.expr.push_back(tokens.at(j));
                    }
                }
            }
            else {
                for (int j = backMovementer + 1; j < forwardMovementer; j++) {
                    auto k = std::find(ignorelist.begin(), ignorelist.end(), tokens.at(j).getValue());
                    if (k == ignorelist.end()) {
                        eipObj.expr.push_back(tokens.at(j));
                    }
                }
            }

            eipObj.area = currentArea;
            eip.push_back(eipObj);
            eipObj.expr.clear();
            i = forwardMovementer;
        }
        else {
            i++;
        }
    }
}

void SemanticAnalyzer::printAllExpressions() {
    for (int i = 0; i < eip.size(); i++) {
        for (int j = 0; j < eip.at(i).expr.size(); j++) {
            std::cout << eip.at(i).expr.at(j).getValue() << " ";
        }
        std::cout << "| Area = " << eip.at(i).area << "\n";
    }
}

void SemanticAnalyzer::doSemanticAnalysis() {
    checkVariableDeclaration();
}

void SemanticAnalyzer::checkVariableDeclaration() {
    for (const auto& expr : eip) {
        for (const auto& token : expr.expr) {
            if (token.getTokenType() == IDENTIFIER) {
                bool isDeclared = false;
                for (const auto& var : allVariables) {
                    if (var.varName == token.getValue() && (var.area == expr.area || var.area == token.getValue())) {
                        isDeclared = true;
                        break;
                    }
                }
                if (!isDeclared) {
                    std::cout << "Error: Variable '" << token.getValue() << "' is not declared in area '" << expr.area << "'\n";
                }
            }
        }
    }
}

void SemanticAnalyzer::checkTypeCompatibility() {
    for (const auto& expr : eip) {
        std::string expectedType;
        for (size_t i = 0; i < expr.expr.size(); ++i) {
            const auto& token = expr.expr[i];
            if (token.getTokenType() == IDENTIFIER) {
                auto it = std::find_if(
                    allVariables.begin(),
                    allVariables.end(),
                    [&token](const varsInProgram& var) {
                        return var.varName == token.getValue();
                    }
                );
                if (it != allVariables.end()) {
                    if (expectedType.empty()) {
                        expectedType = it->varType;
                    }
                    else if (expectedType != it->varType) {
                        std::cout << "Error: Type mismatch for variable '" << token.getValue() << "'." << std::endl;
                    }
                }
            }
            else if (token.getTokenType() == OPERATOR) {
                if (token.getValue() == "+" || token.getValue() == "-" || token.getValue() == "*" || token.getValue() == "/") {
                    if (i > 0 && i < expr.expr.size() - 1) {
                        const auto& left = expr.expr[i - 1];
                        const auto& right = expr.expr[i + 1];
                        std::string leftType, rightType;

                        auto findType = [&](const Token& tok) -> std::string {
                            if (tok.getTokenType() == IDENTIFIER) {
                                auto varIt = std::find_if(
                                    allVariables.begin(),
                                    allVariables.end(),
                                    [&tok](const varsInProgram& var) {
                                        return var.varName == tok.getValue();
                                    }
                                );
                                if (varIt != allVariables.end()) {
                                    return varIt->varType;
                                }
                            }
                            return tok.getTokenType() == INT ? "integer" :
                                tok.getTokenType() == FLOAT ? "real" : "";
                            };

                        leftType = findType(left);
                        rightType = findType(right);

                        if (!leftType.empty() && !rightType.empty() && leftType != rightType) {
                            std::cout << "Error: Type mismatch in expression involving '"
                                << left.getValue() << "' and '" << right.getValue() << "'." << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void SemanticAnalyzer::checkDivisionByZero() {
    for (const auto& expr : eip) {
        for (size_t i = 0; i < expr.expr.size(); ++i) {
            const auto& token = expr.expr[i];
            if (token.getValue() == "/" && i < expr.expr.size() - 1) {
                const auto& nextToken = expr.expr[i + 1];

                bool isZero = (nextToken.getTokenType() == INT && nextToken.getValue() == "0");

                if (!isZero && nextToken.getTokenType() == IDENTIFIER) {
                    auto varIt = std::find_if(
                        allVariables.begin(),
                        allVariables.end(),
                        [&nextToken](const varsInProgram& var) {
                            return var.varName == nextToken.getValue();
                        }
                    );
                    if (varIt != allVariables.end() && varIt->varType == "integer") {
                        // Здесь нужно будет добавить проверку значения переменной, если оно хранится
                        isZero = false; // Условно (если будет система отслеживания значений).
                    }
                }

                if (isZero) {
                    std::cout << "Error: Division by zero detected in expression." << std::endl;
                }
            }
        }
    }
}
