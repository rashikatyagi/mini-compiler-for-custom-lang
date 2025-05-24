# mini-compiler-for-custom-language
## Overview
This project implements a mini compiler in C++ for a custom programming language featuring unique syntax (e.g., === for assignment). It covers lexical analysis, LL(1) parsing, symbol table construction, and semantic analysis with type checking. Supports integer, decimal, and string types, along with control flow statements (if, while, for).
## Features
- Lexer for tokenizing source code
- LL(1) parser for syntax analysis
- Symbol table for managing variable declarations and scope
- Semantic analyzer for type checking and error detection
- Custom syntax with strict assignment operator (===)
- Basic control flow handling
## Prerequisites
- C++ compiler (e.g., g++)
- C++17 or later recommended
## Build & Run
### For error handling and intermediate code generation
```bash
g++ -std=c++17 lexer.cpp parser.cpp symbol_table.cpp main.cpp -o mini_compiler
./mini_compiler
```
### For assembly language generation
```bash
g++ asm_generator.cpp -o asmgen
./asmgen
```
