#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>  // For std::setw

// Macro Definition Table (MDT)
std::vector<std::string> mdt;  // Stored as a vector of strings

// Add instruction to MDT
void addToMDT(const std::string& instruction) {
    mdt.push_back(instruction);
}

// Print the Macro Definition Table (MDT)
void printMDT() {
    std::cout << "\nMacro Definition Table (MDT):\n";
    std::cout << std::setw(15) << "MDT Index" << std::setw(30) << "Instruction" << "\n";  // Set column width
    for (size_t i = 0; i < mdt.size(); i++) {
        std::cout << std::setw(15) << i << std::setw(30) << mdt[i] << "\n";
    }
}

// Process the input file to extract macro definitions and add them to MDT
void processInputFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    bool insideMacro = false;
    
    while (getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string word;
        ss >> word;

        if (word == "-") {
            // Skip lines starting with "-"
            continue;
        }

        if (word == "MACRO") {
            insideMacro = true;
            continue;  // Skip the "MACRO" keyword itself
        }
        
        if (word == "END") {
            insideMacro = false;
            continue;  // Skip the "END" keyword itself
        }

        if (insideMacro) {
            // Add the instruction to MDT while inside a macro definition
            addToMDT(line);
        }
    }

    inputFile.close();
}

int main() {
    std::string filename;
    
    // Ask for the input file name
    std::cout << "Enter the input file name: ";
    std::cin >> filename;

    // Process the input file and generate MDT
    processInputFile(filename);

    // Display MDT after processing the file
    printMDT();
    
    return 0;
}
