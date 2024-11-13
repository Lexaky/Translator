#include "TokenTypes.h"

const std::map<std::string, std::string> TokenTypes::ACCESS_MODIFIERS{
    {"public", "PUBLIC"},
    {"private", "PRIVATE"},
    {"protected", "PROTECTED"}
};

const std::map<std::string, std::string> TokenTypes::KEY_WORDS{
    {"class", "CLASS"},
    {"else", "ELSE"},
    {"for", "FOR"},
    {"if", "IF"},
    {"new", "NEW"},
    {"package", "PACKAGE"},
    {"return", "RETURN"},
    {"static", "STATIC"},
    {"this", "THIS"},
    {"while", "WHILE"},
    {"System.out.println", "SYSTEM.OUT.PRINTLN"},
    {"switch", "SWITCH"},
    {"case", "CASE"},
    {"default", "DEFAULT"}
};

const std::map<std::string, std::string> TokenTypes::DATA_TYPES{
    {"byte", "BYTE"},
    {"short", "SHORT"},
    {"int", "INT"},
    {"long", "LONG"},
    {"float", "FLOAT"},
    {"double", "DOUBLE"},
    {"boolean", "BOOLEAN"},
    {"char", "CHAR"},
    {"String", "STRING"},
    {"void", "VOID"}
};

const std::map<std::string, std::string> TokenTypes::SPECIAL_SYMBOLS{
    {"(", "LEFT_BRACKET"},
    {")", "RIGHT_BRACKET"},
    {"[", "LEFT_SQUARE_BRACKET"},
    {"]", "RIGHT_SQUARE_BRACKET"},
    {"{", "LEFT_CURLY_BRACKET"},
    {"}", "RIGHT_CURLY_BRACKET"},
    {";", "SEMICOLON"},
    {",", "COMMA"}
};

const std::map<std::string, std::string> TokenTypes::OPERATORS{
    {"=", "ASSIGN"},
    {"==", "IS_EQUAL"},
    {"!=", "INEQUALITY"},
    {"<", "LESS"},
    {">", "MORE"},
    {">=", "GREATE_THAN_OR_EQUAL"},
    {"<=", "LESS_THAN_OR_EQUAL"},
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"*", "MULTIPLY"},
    {"/", "DIVIDE"},
    {"%", "MOD"},
    {"++", "INCREMENT"},
    {"--", "DECREMENT"},
    {"+=", "EDITION"},
    {"-=", "SUBSTRACTION"},
    {"*=", "MULTIPLACATION"},
    {"/=", "DIVISION"},
    {"%=", "MODULUS"},
    {"&&", "AND"},
    {"&", "SHORT_AND"},
    {"||", "OR"},
    {"|", "SHORT_OR"},
    {"!", "NOT"},
    {":", "DOUBLE_DOT"}
};

const std::map<std::string, char> TokenTypes::IGNORE{
    {"\n", '\0'},
    {" ", '\0'},
    {"\t", '\0'}
};