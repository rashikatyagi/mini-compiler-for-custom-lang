// #include "intermediate_code_generator.h"
// #include <iostream>
// #include <sstream>
// #include <string>

// std::string IntermediateCodeGenerator::newTemp() {
//     return "t" + std::to_string(tempCount++);
// }

// std::string IntermediateCodeGenerator::newLabel() {
//     return "L" + std::to_string(labelCount++);
// }

// void IntermediateCodeGenerator::emit(const std::string& res, const std::string& arg1, const std::string& op, const std::string& arg2) {
//     code.emplace_back(res, arg1, op, arg2);
// }

// void IntermediateCodeGenerator::emitLabel(const std::string& label) {
//     code.emplace_back(label + ":", "", "", "");
// }

// // std::string IntermediateCodeGenerator::generateIfCondition(const std::string& cond) {
// //     std::string temp = newTemp();

// //     // Basic parsing assuming condition is always in form: lhs op rhs (e.g., "x < 7")
// //     std::istringstream iss(cond);
// //     std::string lhs, op, rhs;
// //     iss >> lhs >> op >> rhs;

// //     if (lhs.empty() || op.empty() || rhs.empty()) {
// //         throw std::runtime_error("Invalid condition format: " + cond);
// //     }

// //     emit(temp, lhs, op, rhs);  // Emit: t0 = lhs op rhs
// //     return temp;
// // }

// std::string IntermediateCodeGenerator::generateIfCondition(const std::string& cond) {
//     std::string temp = newTemp();

//     std::istringstream iss(cond);
//     std::string lhs, op, rhs;
//     iss >> lhs >> op >> rhs;

//     // Emit a proper three-address comparison expression: t0 = x < 7
//     emit(temp, lhs, op, rhs);

//     return temp;
// }


// void IntermediateCodeGenerator::generateIfElse(const std::string& cond, const std::string& trueLabel, const std::string& falseLabel) {
//     std::string condTemp = generateIfCondition(cond);
//     emit("ifFalse", condTemp, "goto", falseLabel);
//     emit("goto", trueLabel);
// }

// void IntermediateCodeGenerator::generateWhileStart(const std::string& startLabel) {
//     emitLabel(startLabel);
// }

// void IntermediateCodeGenerator::generateWhileCondition(const std::string& cond, const std::string& startLabel, const std::string& endLabel) {
//     std::string condTemp = generateIfCondition(cond);
//     emit("ifFalse", condTemp, "goto", endLabel);
// }

// void IntermediateCodeGenerator::generateForInit(const std::string& init) {
//     emit(init, "");
// }

// void IntermediateCodeGenerator::generateForCondition(const std::string& cond, const std::string& startLabel, const std::string& endLabel) {
//     std::string condTemp = generateIfCondition(cond);
//     emit("ifFalse", condTemp, "goto", endLabel);
// }

// void IntermediateCodeGenerator::generateForIncrement(const std::string& incr, const std::string& startLabel) {
//     emit(incr, "");
//     emit("goto", startLabel);
// }

// void IntermediateCodeGenerator::printCode() {
//     for (const auto& instr : code) {
//         if (instr.op.empty() && instr.arg2.empty() && !instr.arg1.empty()) {
//             std::cout << instr.result << " " << instr.arg1 << std::endl;
//         } else if (instr.op.empty() && instr.arg1.empty() && instr.arg2.empty()) {
//             std::cout << instr.result << std::endl;
//         } else if (instr.op == "goto") {
//             std::cout << instr.op << " " << instr.arg2 << std::endl;
//         } else if (instr.result == "ifFalse") {
//             std::cout << "ifFalse " << instr.arg1 << " goto " << instr.arg2 << std::endl;
//         } 
//         // Modified part: For assignment with op "=", no arg2
//         else if (instr.op == "=" && instr.arg2.empty()) {
//             std::cout << instr.result << " = " << instr.arg1 << std::endl;
//         }
//         else {
//             std::cout << instr.result << " = " << instr.arg1 << " " << instr.op << " " << instr.arg2 << std::endl;
//         }
//     }
// }

// void IntermediateCodeGenerator::writeToFile(const std::string& filename) {
//     std::ofstream outfile(filename);
//     for (const auto& instr : code) {
//         if (instr.op.empty() && instr.arg2.empty() && !instr.arg1.empty()) {
//             outfile << instr.result << " " << instr.arg1 << "\n";
//         } else if (instr.op.empty() && instr.arg1.empty() && instr.arg2.empty()) {
//             outfile << instr.result << "\n";
//         } else if (instr.op == "goto") {
//             outfile << instr.op << " " << instr.arg2 << "\n";
//         } else if (instr.result == "ifFalse") {
//             outfile << "ifFalse " << instr.arg1 << " goto " << instr.arg2 << "\n";
//         }
//         // Modified part: For assignment with op "=", no arg2
//         else if (instr.op == "=" && instr.arg2.empty()) {
//             outfile << instr.result << " = " << instr.arg1 << "\n";
//         }
//         else {
//             outfile << instr.result << " = " << instr.arg1 << " " << instr.op << " " << instr.arg2 << "\n";
//         }
//     }
// }

#include "intermediate_code_generator.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string IntermediateCodeGenerator::newTemp() {
    return "t" + std::to_string(tempCount++);
}

std::string IntermediateCodeGenerator::newLabel() {
    return "L" + std::to_string(labelCount++);
}

void IntermediateCodeGenerator::emit(const std::string& res, const std::string& arg1, const std::string& op, const std::string& arg2) {
    code.emplace_back(res, arg1, op, arg2);
}

void IntermediateCodeGenerator::emitLabel(const std::string& label) {
    code.emplace_back(label + ":", "", "", "");
}

std::string IntermediateCodeGenerator::generateIfCondition(const std::string& cond) {
    std::string temp = newTemp();

    // Split the condition into lhs, operator, and rhs
    std::istringstream iss(cond);
    std::string lhs, op, rhs;
    iss >> lhs >> op >> rhs;

    if (lhs.empty() || op.empty() || rhs.empty()) {
        throw std::runtime_error("Invalid condition format: " + cond);
    }

    emit(temp, lhs, op, rhs);  // e.g., t0 = x < 7
    return temp;
}

void IntermediateCodeGenerator::generateIfElse(const std::string& cond, const std::string& trueLabel, const std::string& falseLabel) {
    std::string condTemp = generateIfCondition(cond);
    emit("ifFalse", condTemp, "goto", falseLabel);
    emit("goto", trueLabel);
}

void IntermediateCodeGenerator::generateWhileStart(const std::string& startLabel) {
    emitLabel(startLabel);
}

void IntermediateCodeGenerator::generateWhileCondition(const std::string& cond, const std::string& startLabel, const std::string& endLabel) {
    std::string condTemp = generateIfCondition(cond);
    emit("ifFalse", condTemp, "goto", endLabel);
}

void IntermediateCodeGenerator::generateForInit(const std::string& init) {
    emit(init, "");
}

void IntermediateCodeGenerator::generateForCondition(const std::string& cond, const std::string& startLabel, const std::string& endLabel) {
    std::string condTemp = generateIfCondition(cond);
    emit("ifFalse", condTemp, "goto", endLabel);
}

void IntermediateCodeGenerator::generateForIncrement(const std::string& incr, const std::string& startLabel) {
    emit(incr, "");
    emit("goto", startLabel);
}

void IntermediateCodeGenerator::printCode() {
    for (const auto& instr : code) {
        if (instr.op.empty() && instr.arg2.empty() && !instr.arg1.empty()) {
            std::cout << instr.result << " " << instr.arg1 << std::endl;
        } else if (instr.op.empty() && instr.arg1.empty() && instr.arg2.empty()) {
            std::cout << instr.result << std::endl;
        } else if (instr.result == "ifFalse") {
            std::cout << "ifFalse " << instr.arg1 << " goto " << instr.arg2 << std::endl;
        } else if (instr.op == "goto") {
            std::cout << "goto " << instr.arg2 << std::endl;
        } else if (instr.op == "=" && instr.arg2.empty()) {
            std::cout << instr.result << " = " << instr.arg1 << std::endl;
        } else {
            std::cout << instr.result << " = " << instr.arg1 << " " << instr.op << " " << instr.arg2 << std::endl;
        }
    }
}

void IntermediateCodeGenerator::writeToFile(const std::string& filename) {
    std::ofstream outfile(filename);
    for (const auto& instr : code) {
        if (instr.op.empty() && instr.arg2.empty() && !instr.arg1.empty()) {
            outfile << instr.result << " " << instr.arg1 << "\n";
        } else if (instr.op.empty() && instr.arg1.empty() && instr.arg2.empty()) {
            outfile << instr.result << "\n";
        } else if (instr.result == "ifFalse") {
            outfile << "ifFalse " << instr.arg1 << " goto " << instr.arg2 << "\n";
        } else if (instr.op == "goto") {
            outfile << "goto " << instr.arg2 << "\n";
        } else if (instr.op == "=" && instr.arg2.empty()) {
            outfile << instr.result << " = " << instr.arg1 << "\n";
        } else {
            outfile << instr.result << " = " << instr.arg1 << " " << instr.op << " " << instr.arg2 << "\n";
        }
    }
}
