#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>  // For std::setw

struct MNTEntry {
    std::string macroName;
    int mdtIndex;  // Corresponds to the index in the Macro Definition Table (MDT)
};

std::unordered_map<std::string, MNTEntry> mnt;  // Macro Name Table
std::vector<std::string> mdt;  // Macro Definition Table (stored as instructions)

void addToMNT(const std::string &macroName, int mdtIndex) {
    MNTEntry newEntry = {macroName, mdtIndex};
    mnt[macroName] = newEntry;
}

void printMNT() {
    std::cout << "\nMacro Name Table (MNT):\n";
    std::cout << std::setw(15) << "Macro Name" << std::setw(15) << "MDT Index" << "\n";  // Set column width
    for (const auto &entry : mnt) {
        std::cout << std::setw(15) << entry.second.macroName << std::setw(15) << entry.second.mdtIndex << "\n";
    }
}

void processInputFile(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    int mdtIndex = 0;
    bool insideMacro = false;
    std::string currentMacro;

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
            ss >> word;
            currentMacro = word;
            addToMNT(currentMacro, mdtIndex);
        } else if (word == "END") {
            insideMacro = false;
        } else if (insideMacro) {
            mdt.push_back(line);  // Add instruction to MDT
            mdtIndex++;
        }
    }
    inputFile.close();
}

int main() {
    std::string filename;
    
    // Ask for the input file name
    std::cout << "Enter the input file name: ";
    std::cin >> filename;

    // Process the input file and generate MNT and MDT
    processInputFile(filename);

    // Display MNT after processing the file
    printMNT();
    
    return 0;
}
