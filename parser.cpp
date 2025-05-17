// parser.cpp
#include "parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

Token Parser::peek() {
    if (current < tokens.size()) 
        return tokens[current];
    // Return a dummy EOF token if out of range
    return Token(TokenType::END_OF_FILE, "");
}

Token Parser::advance() {
    if (current < tokens.size()) current++;
    return tokens[current - 1];
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    return current < tokens.size() && tokens[current].type == type;
}

void Parser::error(const std::string& msg) {
    std::cerr << "Syntax Error: " << msg << " at token: " << peek().lexeme << std::endl;
    exit(1);
}

void Parser::parse() {
    program();
    std::cout << "Parsing successful!" << std::endl;
}

void Parser::program() {
    while (current < tokens.size() && peek().type != TokenType::END_OF_FILE) {
        statement();
    }
}

void Parser::statement() {
    if (check(TokenType::INTEGER_TYPE) || check(TokenType::DECIMAL_TYPE) || check(TokenType::STRING_TYPE)) {
    varDeclaration();
    } else if (check(TokenType::IF)) {
        ifStatement();
    } else if (check(TokenType::WHILE)) {
        whileStatement();
    } else if (check(TokenType::FOR)) {
        forStatement();
    } else if (check(TokenType::PRINT)) {
        printStatement();
    } else if (check(TokenType::IDENTIFIER)) {
        assignment();
    } else {
        error("Unexpected statement");
    }
}

void Parser::varDeclaration() {
    // Determine and consume type token
    std::string type;
    if (check(TokenType::INTEGER_TYPE)) {
        type = "integer";
        advance();
    } else if (check(TokenType::DECIMAL_TYPE)) {
        type = "decimal";
        advance();
    } else if (check(TokenType::STRING_TYPE)) {
        type = "string";
        advance();
    } else {
        error("Expected type");
    }

    // Identifier
    if (!check(TokenType::IDENTIFIER)) {
        error("Expected identifier");
    }
    std::string varName = peek().lexeme;
    advance();

    // Check for ===
    if (!match(TokenType::ASSIGN)) {
        error("Expected ===");
    }

    // Check value type
    if (!check(TokenType::NUMBER) && !check(TokenType::STRING_LITERAL)) {
        error("Expected value");
    }

    std::string value = peek().lexeme;
    TokenType valueType = peek().type;
    advance(); // consume value

    // Type checking: Ensure assigned value matches declared type
    if (type == "integer" || type == "decimal") {
        if (valueType != TokenType::NUMBER) {
            error("Type mismatch: Expected number");
        }
    } else if (type == "string") {
        if (valueType != TokenType::STRING_LITERAL) {
            error("Type mismatch: Expected string literal");
        }
    }

    // Semicolon
    if (!match(TokenType::SEMICOLON)) {
        error("Expected semicolon");
    }

    // Check for re-declaration
    if (symTable.exists(varName)) {
        error("Variable '" + varName + "' already declared");
    }

    // Add to symbol table
    symTable.insert(varName, type);
}

void Parser::assignment() {
    std::string varName;

    // Expect and store the identifier
    if (check(TokenType::IDENTIFIER)) {
        varName = peek().lexeme;

        // Semantic Check 1: Is variable declared?
        if (!symTable.exists(varName)) {
            error("Undeclared variable: " + varName);
        }

        advance(); // consume identifier
    } else {
        error("Expected variable name");
    }

    // Match ===
    if (!match(TokenType::ASSIGN)) {
        error("Expected ===");
    }

    // Now, check the type of the value being assigned
    std::string assignedType;
    if (check(TokenType::NUMBER)) {
        const std::string& lex = peek().lexeme;
        assignedType = (lex.find('.') != std::string::npos) ? "decimal" : "integer";
        advance();
    } else if (check(TokenType::STRING_LITERAL)) {
        assignedType = "string";
        advance();
    } else {
        error("Unsupported assignment type");
    }

    // Semantic Check 2: Type compatibility
    std::string declaredType = symTable.getType(varName);
    if (declaredType != assignedType) {
        error("Type mismatch in assignment to '" + varName + "': expected " + declaredType + ", got " + assignedType);
    }

    // Expect semicolon
    if (!match(TokenType::SEMICOLON)) {
        error("Expected semicolon");
    }
}


void Parser::ifStatement() {
    advance(); // consume 'if'

    if (!match(TokenType::QUOTED_CONDITION) && !match(TokenType::STRING_LITERAL)) {
        error("Expected string condition");
    }

    block();

    if (match(TokenType::ELSE)) {
        block();
    }
}

void Parser::whileStatement() {
    advance(); // consume 'while'

    if (!match(TokenType::QUOTED_CONDITION) && !match(TokenType::STRING_LITERAL)) {
        error("Expected string condition");
    }

    block();
}

void Parser::forStatement() {
    advance(); // consume 'for'

    if (!match(TokenType::QUOTED_CONDITION) && !match(TokenType::STRING_LITERAL)) {
        error("Expected for condition in string");
    }

    block();
}

void Parser::block() {
    if (!match(TokenType::LBRACE)) {
        error("Expected {");
    }

    while (!check(TokenType::RBRACE)) {
        if (peek().type == TokenType::END_OF_FILE) {
            error("Expected } before end of input");
        }
        statement();
    }

    if (!match(TokenType::RBRACE)) {
        error("Expected }");
    }
}

void Parser::printStatement() {
    advance(); // consume 'print'

    if (!check(TokenType::STRING_LITERAL) && !check(TokenType::IDENTIFIER)) {
        error("Expected string literal or variable");
    }
    advance();

    if (!match(TokenType::SEMICOLON)) {
        error("Expected semicolon");
    }
}
