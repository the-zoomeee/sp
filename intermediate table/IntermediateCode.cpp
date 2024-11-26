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

    std::ofstream icFile("IntermediateCode.txt");
    if (!icFile) {
        std::cerr << "Error: Unable to create \"IntermediateCode.txt\".\n";
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
            icFile << "AD 01 " << operand << "\n";
        } else if (opcode == "END") {
            icFile << "AD 02 -\n";
            break;
        } else if (opcode == "DS" || opcode == "DC") {
            icFile << "DL " << (opcode == "DS" ? "01" : "02") << " " << operand << "\n";
            LC += std::stoi(operand);
        } else {
            icFile << "IS 01 " << opcode << ", " << operand << "\n";
            LC++;
        }
    }

    std::cout << "Intermediate Code written to \"IntermediateCode.txt\".\n";

    inputFile.close();
    icFile.close();

    return 0;
}
