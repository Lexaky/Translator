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
    bool isType = false;         
    std::string currentType;     
    std::string currentArea = "GLOBAL"; 
    std::string currentFunctionOrMethod; 

    for (size_t i = 0; i < sourceTokens.size(); ++i) {
        const Token& token = sourceTokens[i];

        
        if (typeMapping.find(token.getValue()) != typeMapping.end()) {
            isType = true;
            currentType = typeMapping[token.getValue()];
        }
        
        else if (isType && token.getTokenType() == IDENTIFIER) {
            const std::string& identifier = token.getValue();

            
            if (i + 1 < sourceTokens.size() && sourceTokens[i + 1].getValue() == "(") {
                if (currentType == "void") {
                    
                    methodName.push_back(identifier);
                    currentArea = identifier;
                }
                else {
                    
                    functionName.push_back(identifier);
                    functionType.push_back(currentType);
                    currentArea = identifier;
                }
                currentFunctionOrMethod = identifier;
                ++i; 

                
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

            isType = false; 
        }
        
        else if (token.getValue() == "{" && token.getTokenType() == SPECIAL_SYMBOL) {
            
        }
        
        else if (token.getValue() == "}" && token.getTokenType() == SPECIAL_SYMBOL) {
            currentArea = "GLOBAL"; 
            currentFunctionOrMethod.clear();
        }

        
        if (!currentFunctionOrMethod.empty()) {
            currentArea = currentFunctionOrMethod;
        }
    }

    
    std::cout << "Program Main;\n";
    for (int i = 0, j = 0; i < varNames.size(); i++) {
        if (area.at(i) == "GLOBAL" || area.at(i) == "main")
        {
            std::cout << "var " << varNames.at(i) + " : " + varTypes.at(i);

            if (varGlobalStartValue.size() > 0 && j < varGlobalStartValue.size() && varGlobalStartValue.at(j) != "_hasNotInit__" && area.at(i) == "GLOBAL") {
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
    
    for (int i = 0; i < functionName.size(); i++) {
        if (functionName.at(i) == "main")
            continue;
        for (int j = 0; j < sourceTokens.size(); j++) {
            if (sourceTokens.at(j).getValue() == functionName.at(i)) {
                while (sourceTokens.at(j).getValue() != "{" || sourceTokens.at(j).getTokenType() != SPECIAL_SYMBOL)
                    j++;
                j++;
                int nestedLevel = 1;
                while (nestedLevel != 0 || sourceTokens.at(j).getValue() != "}" || sourceTokens.at(j).getTokenType() != SPECIAL_SYMBOL) {
                    bodyFunction.push_back(sourceTokens.at(j));
                    if (sourceTokens.at(j).getValue() == "{" && sourceTokens.at(j).getTokenType() == SPECIAL_SYMBOL) {
                        nestedLevel++;
                    }
                    j++;
                    if (sourceTokens.at(j).getValue() == "}" && nestedLevel > 0 && sourceTokens.at(j).getTokenType() == SPECIAL_SYMBOL) {
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
                break;
            }
        }
    }
    
    bodyFunction.clear();
    varNamesInFunction.clear();
    varTypesInFunction.clear();
    bodyFunction.reserve(sourceTokens.size());
    
    for (int i = 0; i < methodName.size(); i++) {
        for (int j = 0; j < sourceTokens.size(); j++) {
            if (sourceTokens.at(j).getValue() == methodName.at(i)) {
                while (sourceTokens.at(j).getValue() != "{")
                    j++;
                j++;
                int nestedLevel = 0;
                while (nestedLevel != 0 || sourceTokens.at(j).getValue() != "}" || sourceTokens.at(j).getTokenType() != SPECIAL_SYMBOL) {
                    if (j < sourceTokens.size() && sourceTokens.at(j).getValue() == "}" && sourceTokens.at(j).getTokenType() == SPECIAL_SYMBOL && nestedLevel > 0) {
                        nestedLevel--;
                    }
                    
                    if (j < sourceTokens.size() && sourceTokens.at(j).getValue() == "{" && sourceTokens.at(j).getTokenType() == SPECIAL_SYMBOL) {
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
            std::cout << varNames.at(i) << " : " << varTypes.at(i).substr(0, varTypes.at(i).size() - 1);
            if (i + 1 > varNames.size())
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
        if (tokens.at(i).getValue() == "return" && tokens.at(i).getTokenType() == KEY_WORD) {
            Token returnToken(functionName, KEY_WORD);
            newTokens.push_back(returnToken);
            Token equalToken("=", OPERATOR);
            newTokens.push_back(equalToken);
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
    
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens.at(i).getValue() == "if" && tokens.at(i).getTokenType() == KEY_WORD) {
            std::vector<Token> ifTokens;
            ifTokens.reserve(tokens.size());
            int j = i, nestedLevel = 0;
            while (j < tokens.size() && tokens.at(j).getValue() != "}" || tokens.at(j).getTokenType() != SPECIAL_SYMBOL || nestedLevel != 0) {
                if (j < tokens.size())
                    ifTokens.push_back(tokens.at(j));
                j++;
                if (j < tokens.size() && tokens.at(j).getValue() == "}" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL && nestedLevel > 0) {
                    nestedLevel--;
                }
                if (j < tokens.size() && tokens.at(j).getValue() == "{" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL) {
                    nestedLevel++;
                }
            }
            if (j < tokens.size())
                ifTokens.push_back(tokens.at(j));
            
            ifGenerator(ifTokens);
            i = j + 1;
            if (i < tokens.size() && tokens.at(i).getValue() != "else") {
                std::cout << ";";
                continue;
            }
            else if (i >= tokens.size()) {
                std::cout << ";\n";
                continue;
            }
            std::cout << "\n";
            ifTokens.clear();
            if (i < tokens.size() && tokens.at(i).getValue() == "else") {
                ifTokens.reserve(tokens.size());
                int j = i, nestedLevel = 0;
                while (j < tokens.size() && tokens.at(j).getValue() != "}" || tokens.at(j).getTokenType() != SPECIAL_SYMBOL || nestedLevel != 0) {
                    if (j < tokens.size())
                        ifTokens.push_back(tokens.at(j));
                    j++;
                    if (j < tokens.size() && tokens.at(j).getValue() == "}" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL && nestedLevel > 0) {
                        nestedLevel--;
                    }
                    if (j < tokens.size() && tokens.at(j).getValue() == "{" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL) {
                        nestedLevel++;
                    }
                }
                if (j < tokens.size())
                    ifTokens.push_back(tokens.at(j));
                elseGenerator(ifTokens);
                i = j;
                std::cout << "\n";
                continue;
            }
        }
        else if (tokens.at(i).getValue() == "while" && tokens.at(i).getTokenType() == KEY_WORD) {
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
        else if (tokens.at(i).getValue() == "for" && tokens.at(i).getTokenType() == KEY_WORD) {
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
        else if (tokens.at(i).getValue() == "++" && tokens.at(i).getTokenType() == OPERATOR) {
            std::cout << ":= " << tokens.at(i - 1).getValue() << " + 1";
            continue;
        }
        else if (tokens.at(i).getValue() == "--" && tokens.at(i).getTokenType() == OPERATOR) {
            std::cout << ":= " << tokens.at(i - 1).getValue() << " - 1";
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
    if (tokens.at(i).getValue() == "if" && tokens.at(i).getTokenType() == KEY_WORD)
        i++;
    std::vector<Token> ifExpression;
    ifExpression.reserve(sourceTokens.size());
    
    
    if (tokens.at(i).getValue() == "(")
        i++;
    int nestedLevel = 1;
    while (nestedLevel != 0) {
        if (tokens.at(i).getValue() == "(" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel++;
        }
        if (tokens.at(i).getValue() == ")" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel--;
            if (nestedLevel == 0)
            {
                break;
            }
        }
        ifExpression.push_back(tokens.at(i));
        i++;
    }
    
    ifExpressionGenerator(ifExpression);
    std::cout << "then begin\n";
    std::vector<Token> insideIf;
    insideIf.reserve(sourceTokens.size());
    i += 2;
    
    int j = tokens.size() - 2; 
    
    for (i; i <= j; i++) {
        insideIf.push_back(tokens.at(i));
    }
    globalGenerator(insideIf);
    std::cout << "end";
}

void TokenMapper::ifExpressionGenerator(std::vector<Token> tokens) {
    std::vector<Token> subExpression;
    subExpression.reserve(tokens.size());
    for (int i = 0; i < tokens.size(); i++) {
        if ((tokens.at(i).getValue() == "&&" || tokens.at(i).getValue() == "||") 
            && tokens.at(i).getTokenType() == OPERATOR) {
            std::cout << "(";
            globalGenerator(subExpression);
            subExpression.clear();
            subExpression.reserve(tokens.size());
            std::cout << ")";
            subExpression.push_back(tokens.at(i));
            globalGenerator(subExpression);
            subExpression.clear();
            subExpression.reserve(tokens.size());
        }
        else {
            subExpression.push_back(tokens.at(i));
        }
    }
    std::cout << "(";
    globalGenerator(subExpression);
    std::cout << ")";
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
    int nestedLevel = 1;
    while (nestedLevel != 0) {
        if (tokens.at(i).getValue() == "(" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel++;
        }
        if (tokens.at(i).getValue() == ")" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel--;
            if (nestedLevel == 0)
            {
                break;
            }
        }
        ifExpression.push_back(tokens.at(i));
        i++;
    }
    i++;
    ifExpressionGenerator(ifExpression);
    std::cout << "do begin\n";
    std::vector<Token> insideIf;
    insideIf.reserve(sourceTokens.size());
    nestedLevel = 0;
    i++;
    while (nestedLevel != 0 || tokens.at(i).getValue() != "}" || tokens.at(i).getTokenType() != SPECIAL_SYMBOL) {
        if (tokens.at(i).getValue() == "}" && nestedLevel > 0 && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel--;
        }
        if (tokens.at(i).getValue() == "{" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
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
    int j = tokens.size() - 2;
    for (i; i <= j; i++) {
        insideElse.push_back(tokens.at(i));
    }
    globalGenerator(insideElse);
    std::cout << "end;\n";
}

void TokenMapper::forGenerator(std::vector<Token> tokens) {
    int i = 1;
    std::vector <Token> forInit;
    forInit.reserve(tokens.size());
    if (tokens.at(i).getValue() == "(")
        i++;
    int nestedLevel = 1;
    while (nestedLevel != 0) {
        if (tokens.at(i).getValue() == "(" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel++;
        }
        if (tokens.at(i).getValue() == ")" && tokens.at(i).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel--;
            if (nestedLevel == 0)
            {
                break;
            }
        }
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
    Token tk(";", SPECIAL_SYMBOL);
    firstPart.push_back(tk);
    for (int k = 0; k < firstPart.size(); k++) {
        if (firstPart.at(k).getValue() == "=" && firstPart.at(k).getTokenType() == OPERATOR) {
            globalGenerator(firstPart);
            break;
        }
    }
    std::cout << "while ";
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
    

    std::vector<Token> insideFor;
    insideFor.reserve(tokens.size());
    
    ifExpressionGenerator(secondPart);
    
    nestedLevel = 1;
    std::cout << "do begin\n";
    if (tokens.at(j).getValue() == ")")
        j++;
    if (tokens.at(j).getValue() == "{" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL)
        j++;
    while ((j < tokens.size() && tokens.at(j).getValue() != "}") || nestedLevel != 0) {
        if (tokens.at(j).getValue() == "{" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL) {
            nestedLevel++;
        }
        if (tokens.at(j).getValue() == "}" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL && nestedLevel - 1 == 0) {
            nestedLevel--;
            insideFor.push_back(tokens.at(j));
            break;
        }
        else if (tokens.at(j).getValue() == "}" && tokens.at(j).getTokenType() == SPECIAL_SYMBOL && nestedLevel > 0) {
            nestedLevel--;
        }
        insideFor.push_back(tokens.at(j));
        j++;
    }
    std::vector<Token> iF; iF.reserve(insideFor.size());
    if (insideFor.at(insideFor.size() - 1).getValue() == "}" &&
        insideFor.at(insideFor.size() - 1).getTokenType() == SPECIAL_SYMBOL) {
        
        for (int k = 0; k < insideFor.size() - 1; k++) {
            iF.push_back(insideFor.at(k));
        }
    }
    globalGenerator(iF);
    
    std::cout << "\n";
    globalGenerator(thirdPart);
    std::cout << ";\n";

    std::cout << "end;";
}

void TokenMapper::printPascalCode() {
	for (int i = 0; i < pascalTokens.size(); i++) {
		std::cout << pascalTokens.at(i) << " ";
	}
}