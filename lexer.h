#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <fstream>

enum class TokenType {
    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,
    ASSIGN,          
    SEMICOLON,
    UNKNOWN,
    OPERATOR,
    COMMA,
    IF,
    ELSE,
    WHILE,
    FOR,
    INTEGER_TYPE,
    DECIMAL_TYPE,
    STRING_TYPE,
    QUOTED_CONDITION,
    PRINT,
    LBRACE,
    RBRACE,
    END_OF_FILE
};


struct Token {
    TokenType type;
    std::string lexeme;
    Token() : type(TokenType::END_OF_FILE), lexeme("") {}
    Token(TokenType t, const std::string& l) : type(t), lexeme(l) {}
};

class Lexer {
private:
    std::string source;
    size_t pos;
    char currentChar;

    void advance();
    void skipWhitespace();
    Token number();
    Token identifierOrKeyword();
    Token quotedCondition();
    Token stringLiteral();

public:
    Lexer(const std::string& src);
    Token getNextToken();
    bool isAtEnd();
};

#endif
