#pragma once

#include <map>
#include <string>

class TokenTypes {
public:
    static const std::map<std::string, std::string> ACCESS_MODIFIERS;
    static const std::map<std::string, std::string> KEY_WORDS;
    static const std::map<std::string, std::string> DATA_TYPES;
    static const std::map<std::string, std::string> SPECIAL_SYMBOLS;
    static const std::map<std::string, std::string> OPERATORS;
    static const std::map<std::string, char> IGNORE;
};