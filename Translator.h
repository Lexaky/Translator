#pragma once

#include <iostream>
#include <string>

#include "Lexer.h"
#include "Parser.h"

class Translator {
public:
    std::string translate(std::string sourceCode);
};
