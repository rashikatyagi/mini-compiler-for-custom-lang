#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void generateAssembly(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile);
    std::ofstream out(outputFile);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Failed to open input or output file.\n";
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        // Label
        if (word.back() == ':') {
            out << word << "\n";
            continue;
        }

        // ifFalse t0 goto L1
        if (word == "ifFalse") {
            std::string condVar, _goto, label;
            iss >> condVar >> _goto >> label;
            out << "cmp " << condVar << ", 0\n";
            out << "je " << label << "\n";
            continue;
        }

        // goto L1
        if (word == "goto") {
            std::string label;
            iss >> label;
            out << "jmp " << label << "\n";
            continue;
        }

        // print statement
        if (word == "print") {
            std::string toPrint;
            std::getline(iss, toPrint);
            // Remove leading whitespace
            toPrint.erase(0, toPrint.find_first_not_of(" \t"));
            if (toPrint.front() != '"' && !isdigit(toPrint.front())) {
                toPrint = "\"" + toPrint + "\"";  // quote string if not already
            }
            out << "print " << toPrint << "\n";
            continue;
        }


        // t1 = a + b
        std::string lhs = word;
        std::string equal, arg1, op, arg2;
        iss >> equal >> arg1;

        if (!(iss >> op)) {
            // Simple assignment: t1 = a
            // Check if arg1 is a literal string (non-numeric, non-variable)
            if (!isdigit(arg1[0]) && !isalpha(arg1[0])) {
                arg1 = "\"" + arg1 + "\"";
            } else if (!isdigit(arg1[0]) && isalpha(arg1[0]) && arg1 != "true" && arg1 != "false") {
                // Treat as string constant
                arg1 = "\"" + arg1 + "\"";
            }
            out << "mov " << lhs << ", " << arg1 << "\n";

            continue;
        }

        // Binary operation: t1 = a + b
        iss >> arg2;
        out << "mov eax, " << arg1 << "\n";

        if (op == "+")
            out << "add eax, " << arg2 << "\n";
        else if (op == "-")
            out << "sub eax, " << arg2 << "\n";
        else if (op == "*")
            out << "imul eax, " << arg2 << "\n";
        else if (op == "/")
            out << "mov edx, 0\nidiv " << arg2 << "\n";  // Simple div without considering signed values
        else if (op == "<")
            out << "cmp " << arg1 << ", " << arg2 << "\nsetl al\nmovzx eax, al\n";
        else if (op == ">")
            out << "cmp " << arg1 << ", " << arg2 << "\nsetg al\nmovzx eax, al\n";
        else if (op == "<=")
            out << "cmp " << arg1 << ", " << arg2 << "\nsetle al\nmovzx eax, al\n";
        else if (op == ">=")
            out << "cmp " << arg1 << ", " << arg2 << "\nsetge al\nmovzx eax, al\n";
        else if (op == "==")
            out << "cmp " << arg1 << ", " << arg2 << "\nsete al\nmovzx eax, al\n";
        else if (op == "!=")
            out << "cmp " << arg1 << ", " << arg2 << "\nsetne al\nmovzx eax, al\n";

        out << "mov " << lhs << ", eax\n";
    }

    in.close();
    out.close();
    std::cout << "Assembly code written to " << outputFile << "\n";
}

int main() {
    generateAssembly("output_3ac.txt", "output_asm.txt");
    return 0;
}
