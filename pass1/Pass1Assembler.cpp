#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    std::string inputFileName;
    std::cout << "Enter the input file name: ";
    std::cin >> inputFileName;

    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file \"" << inputFileName << "\".\n";
        return 1;
    }

    std::string line;
    int LC = 0;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string label, opcode, operand;
        iss >> label >> opcode >> operand;

        if (opcode == "START") {
            LC = std::stoi(operand);
            std::cout << "LC initialized to: " << LC << "\n";
        } else if (opcode == "END") {
            std::cout << "End of program reached.\n";
            break;
        } else if (opcode == "DS" || opcode == "DC") {
            LC += std::stoi(operand);
            std::cout << "LC updated to: " << LC << "\n";
        } else {
            LC++;
            std::cout << "LC incremented to: " << LC << "\n";
        }
    }

    inputFile.close();
    return 0;
}
