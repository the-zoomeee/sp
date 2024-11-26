#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class ErrorHandler {
public:
    void invalidInstruction(const std::string& instruction) {
        std::cerr << "Error: Invalid instruction \"" << instruction << "\".\n";
    }

    void invalidRegister(const std::string& reg) {
        std::cerr << "Error: Invalid register \"" << reg << "\".\n";
    }

    void undefinedSymbol(const std::string& symbol) {
        std::cerr << "Error: Undefined symbol \"" << symbol << "\".\n";
    }
};

int main() {
    ErrorHandler errHandler;

    std::string inputFileName;
    std::cout << "Enter the input file name: ";
    std::cin >> inputFileName;

    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file \"" << inputFileName << "\".\n";
        return 1;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string label, opcode, operand;
        iss >> label >> opcode >> operand;

        if (opcode != "START" && opcode != "END" && opcode != "DS" && opcode != "DC" &&
            opcode != "MOV" && opcode != "ADD" && opcode != "SUB") {
            errHandler.invalidInstruction(opcode);
        }
    }

    inputFile.close();
    return 0;
}
