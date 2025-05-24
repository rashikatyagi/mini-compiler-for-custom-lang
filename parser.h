// parser.h
#pragma once
#include "lexer.h"
#include "symbol_table.h"
#include "intermediate_code_generator.h"



class Parser {
    std::vector<Token> tokens;
    size_t current = 0;

    Token peek();
    Token advance();
    SymbolTable symTable;
    IntermediateCodeGenerator icg;
    bool match(TokenType type);
    bool check(TokenType type);
    void error(const std::string& msg);

public:
    Parser(const std::vector<Token>& tokens);
    void parse(); // Entry point

    IntermediateCodeGenerator& getICG();

private:
    void program();
    void statement();
    void varDeclaration();
    void ifStatement();
    void whileStatement();
    void forStatement();
    void assignment();
    void block();
    void printStatement();
};
