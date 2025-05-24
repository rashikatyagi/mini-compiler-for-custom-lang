#pragma once
#include <string>
#include <vector>
#include <fstream>

struct Instruction {
    std::string result, arg1, op, arg2;
    Instruction(const std::string& r, const std::string& a1, const std::string& o, const std::string& a2)
        : result(r), arg1(a1), op(o), arg2(a2) {}
};

class IntermediateCodeGenerator {
    std::vector<Instruction> code;
    int tempCount = 0;
    int labelCount = 0;

public:
    std::string newTemp();
    std::string newLabel();

    void emit(const std::string& res, const std::string& arg1, const std::string& op = "", const std::string& arg2 = "");
    void emitLabel(const std::string& label);

    std::string generateIfCondition(const std::string& cond);
    void generateIfElse(const std::string& cond, const std::string& trueLabel, const std::string& falseLabel);
    void generateWhileStart(const std::string& startLabel);
    void generateWhileCondition(const std::string& cond, const std::string& startLabel, const std::string& endLabel);
    void generateForInit(const std::string& init);
    void generateForCondition(const std::string& cond, const std::string& startLabel, const std::string& endLabel);
    void generateForIncrement(const std::string& incr, const std::string& startLabel);

    void printCode();
    void writeToFile(const std::string& filename);
};