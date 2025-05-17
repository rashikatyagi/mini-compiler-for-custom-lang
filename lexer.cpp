#include "lexer.h"

// Helper function to check if char is part of operator
bool isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == '=' || c == '!';
}

Lexer::Lexer(const std::string& src) : source(src), pos(0) {
    currentChar = pos < source.size() ? source[pos] : '\0';
}

void Lexer::advance() {
    pos++;
    currentChar = pos < source.size() ? source[pos] : '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(currentChar)) {
        advance();
    }
}

Token Lexer::number() {
    std::string result;
    bool hasDot = false;
    while (isdigit(currentChar) || currentChar == '.') {
        if (currentChar == '.') {
            if (hasDot) break; // only one dot allowed
            hasDot = true;
        }
        result += currentChar;
        advance();
    }
    return Token(TokenType::NUMBER, result);
}

Token Lexer::identifierOrKeyword() {
    std::string result;
    while (isalnum(currentChar) || currentChar == '_') {
        result += currentChar;
        advance();
    }

    // Check keywords
    if (result == "integer") return Token(TokenType::INTEGER_TYPE, result);
    if (result == "decimal") return Token(TokenType::DECIMAL_TYPE, result);
    if (result == "string") return Token(TokenType::STRING_TYPE, result);
    if (result == "if") return Token(TokenType::IF, result);
    if (result == "else") return Token(TokenType::ELSE, result);
    if (result == "while") return Token(TokenType::WHILE, result);
    if (result == "for") return Token(TokenType::FOR, result);
    if (result == "print") return Token(TokenType::PRINT, result);

    return Token(TokenType::IDENTIFIER, result);
}

Token Lexer::quotedCondition() {
    std::string result;
    advance(); // skip opening quote
    while (currentChar != '"' && currentChar != '\0') {
        result += currentChar;
        advance();
    }
    if (currentChar == '"') advance(); // skip closing quote
    return Token(TokenType::QUOTED_CONDITION, result);
}

Token Lexer::stringLiteral() {
    std::string result;
    advance(); // skip opening quote
    while (currentChar != '"' && currentChar != '\0') {
        result += currentChar;
        advance();
    }
    if (currentChar == '"') advance(); // skip closing quote
    return Token(TokenType::STRING_LITERAL, result); // ✅ important
}

Token Lexer::getNextToken() {
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        // Handle assignment operator ===
        if (currentChar == '=') {
            if (pos + 2 < source.size() && source[pos] == '=' && source[pos+1] == '=' && source[pos+2] == '=') {
                pos += 3;
                currentChar = pos < source.size() ? source[pos] : '\0';
                return Token(TokenType::ASSIGN, "===");
            } else {
                // Unknown single '=' - invalid in your language
                advance();
                return Token(TokenType::UNKNOWN, "=");
            }
        }

        // Semicolon
        if (currentChar == ';') {
            advance();
            return Token(TokenType::SEMICOLON, ";");
        }

        // Comma
        if (currentChar == ',') {
            advance();
            return Token(TokenType::COMMA, ",");
        }

        // Braces
        if (currentChar == '{') {
            advance();
            return Token(TokenType::LBRACE, "{");
        }
        if (currentChar == '}') {
            advance();
            return Token(TokenType::RBRACE, "}");
        }

        // Double-quoted strings (now always as STRING_LITERAL)
        if (currentChar == '"') {
            return stringLiteral();
        }

        // Number (integer or decimal)
        if (isdigit(currentChar)) {
            return number();
        }

        // Identifier or keyword
        if (isalpha(currentChar) || currentChar == '_') {
            return identifierOrKeyword();
        }

        // Operators (+, -, *, /, <, >, <=, >=, ==, !=)
        if (isOperatorChar(currentChar)) {
            std::string op;
            op += currentChar;
            advance();
            // Check for two-char operators
            if ((op[0] == '<' || op[0] == '>' || op[0] == '=' || op[0] == '!') && currentChar == '=') {
                op += currentChar;
                advance();
            }
            return Token(TokenType::OPERATOR, op);
        }

        // Unknown character
        std::string unknownChar(1, currentChar);
        advance();
        return Token(TokenType::UNKNOWN, unknownChar);
    }

    return Token(TokenType::END_OF_FILE, "");
}
