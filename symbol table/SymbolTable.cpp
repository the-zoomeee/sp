#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

class SymbolTable {
    std::unordered_map<std::string, int> table;

public:
    void addSymbol(const std::string& symbol, int address) {
        if (table.find(symbol) == table.end()) {
            table[symbol] = address;
        } else {
            std::cerr << "Error: Symbol \"" << symbol << "\" already defined.\n";
        }
    }

    void display() {
        std::cout << "Symbol Table:\n";
        std::cout << "Symbol\tAddress\n";
        for (const auto& entry : table) {
            std::cout << entry.first << "\t" << entry.second << "\n";
        }
    }
};

int main() {
    SymbolTable symTable;

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

        if (!label.empty() && label != "-") {
            symTable.addSymbol(label, LC);
        }

        if (opcode == "START") {
            LC = std::stoi(operand);
        } else if (opcode == "DS" || opcode == "DC") {
            LC += std::stoi(operand);
        } else {
            LC++;
        }
    }

    symTable.display();

    inputFile.close();
    return 0;
}
