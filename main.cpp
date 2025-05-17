#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

int main() {
    std::ifstream file("input.custom");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.custom" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    // Step 1: Lexical Analysis
    Lexer lexer(buffer.str());
    std::vector<Token> tokens;
    Token token;
    do {
        token = lexer.getNextToken();
        tokens.push_back(token);
    } while (token.type != TokenType::END_OF_FILE);

    // Step 2: Syntax + Semantic Analysis
    try {
        Parser parser(tokens);
        parser.parse();
        std::cout << "Parsing and semantic analysis successful!" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error during parsing: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
