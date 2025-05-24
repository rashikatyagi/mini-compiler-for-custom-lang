// parser.cpp
#include "parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

Token Parser::peek() {
    if (current < tokens.size()) 
        return tokens[current];
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
    std::cout << "Parsing successful!\n";

    // After parsing, print and write the intermediate code
    icg.printCode();
    icg.writeToFile("output.tac");
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

    if (!check(TokenType::IDENTIFIER)) error("Expected identifier");
    std::string varName = peek().lexeme;
    advance();

    if (!match(TokenType::ASSIGN)) error("Expected ===");

    if (!check(TokenType::NUMBER) && !check(TokenType::STRING_LITERAL)) error("Expected value");
    std::string value = peek().lexeme;
    TokenType valueType = peek().type;
    advance();

    if ((type == "integer" || type == "decimal") && valueType != TokenType::NUMBER) {
        error("Type mismatch: Expected number");
    } else if (type == "string" && valueType != TokenType::STRING_LITERAL) {
        error("Type mismatch: Expected string literal");
    }

    if (!match(TokenType::SEMICOLON)) error("Expected semicolon");

    if (symTable.exists(varName)) error("Variable '" + varName + "' already declared");
    symTable.insert(varName, type);

    icg.emit(varName, value, "=");
}

void Parser::assignment() {
    std::string varName;

    if (check(TokenType::IDENTIFIER)) {
        varName = peek().lexeme;
        if (!symTable.exists(varName)) error("Undeclared variable: " + varName);
        advance();
    } else {
        error("Expected variable name");
    }

    if (!match(TokenType::ASSIGN)) error("Expected ===");

    std::string assignedType, value;
    if (check(TokenType::NUMBER)) {
        value = peek().lexeme;
        assignedType = (value.find('.') != std::string::npos) ? "decimal" : "integer";
        advance();
    } else if (check(TokenType::STRING_LITERAL)) {
        value = peek().lexeme;
        assignedType = "string";
        advance();
    } else {
        error("Unsupported assignment type");
    }

    std::string declaredType = symTable.getType(varName);
    if (declaredType != assignedType) {
        error("Type mismatch in assignment to '" + varName + "': expected " + declaredType + ", got " + assignedType);
    }

    if (!match(TokenType::SEMICOLON)) error("Expected semicolon");

    icg.emit(varName, value, "=");
}

void Parser::ifStatement() {
    advance(); // consume 'if'

    if (!check(TokenType::QUOTED_CONDITION) && !check(TokenType::STRING_LITERAL))
        error("Expected string condition");

    std::string cond = peek().lexeme;
    advance();

    std::string trueLabel = icg.newLabel();
    std::string falseLabel = icg.newLabel();
    std::string endLabel = icg.newLabel();

    std::string condTemp = icg.generateIfCondition(cond);
    icg.emit("ifFalse", condTemp, "goto", falseLabel);
    icg.emit("goto", trueLabel);

    icg.emitLabel(trueLabel);
    block();

    if (match(TokenType::ELSE)) {
        icg.emit("goto", endLabel);
        icg.emitLabel(falseLabel);
        block();
        icg.emitLabel(endLabel);
    } else {
        icg.emitLabel(falseLabel);
    }
}

void Parser::whileStatement() {
    advance(); // consume 'while'

    if (!check(TokenType::QUOTED_CONDITION) && !check(TokenType::STRING_LITERAL))
        error("Expected string condition");

    std::string cond = peek().lexeme;
    advance();

    std::string startLabel = icg.newLabel();
    std::string endLabel = icg.newLabel();

    icg.generateWhileStart(startLabel);
    icg.generateWhileCondition(cond, startLabel, endLabel);

    block(); // the body of the while loop

    icg.emit("goto", startLabel);  // jump back to condition
    icg.emitLabel(endLabel);       // loop end
}


void Parser::forStatement() {
    advance(); // consume 'for'

    if (!check(TokenType::QUOTED_CONDITION) && !check(TokenType::STRING_LITERAL))
        error("Expected for condition in string");

    std::string cond = peek().lexeme;
    advance();

    // For simplicity: assume for-loop cond is like "x = 0; x < 10; x = x + 1"
    // Parse init, condition, increment manually or assume separate statements. 
    // Here we just emit condition checks and loop structure for demo:

    std::string startLabel = icg.newLabel();
    std::string endLabel = icg.newLabel();

    icg.emitLabel(startLabel);
    std::string condTemp = icg.generateIfCondition(cond);
    icg.emit("ifFalse", condTemp, "goto", endLabel);

    block();

    // Increment statement should be parsed from cond ideally, here omitted for brevity
    icg.emit("goto", startLabel);
    icg.emitLabel(endLabel);
}

void Parser::block() {
    if (!match(TokenType::LBRACE)) error("Expected {");

    while (!check(TokenType::RBRACE)) {
        if (peek().type == TokenType::END_OF_FILE) error("Expected } before end of input");
        statement();
    }

    if (!match(TokenType::RBRACE)) error("Expected }");
}

void Parser::printStatement() {
    advance(); // consume 'print'

    if (!check(TokenType::STRING_LITERAL) && !check(TokenType::IDENTIFIER))
        error("Expected string literal or variable");
    
    std::string toPrint = peek().lexeme;
    advance();

    if (!match(TokenType::SEMICOLON)) error("Expected semicolon");

    // Emit print statement intermediate code as: print toPrint
    icg.emit("print", toPrint);
}

IntermediateCodeGenerator& Parser::getICG() {
    return icg;
}
