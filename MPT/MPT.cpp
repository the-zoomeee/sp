#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>  // For std::setw

// Macro Parameter Table (MPT)
struct MPTEntry {
    int paramCount;  // Number of parameters in the macro
    std::vector<std::string> parameters;  // List of parameter names
};

std::unordered_map<std::string, MPTEntry> mpt;  // Macro Parameter Table

// Function to add an entry to the MPT
void addToMPT(const std::string& macroName, const std::vector<std::string>& params) {
    MPTEntry newEntry = {static_cast<int>(params.size()), params};
    mpt[macroName] = newEntry;
}

// Function to print the Macro Parameter Table (MPT)
void printMPT() {
    std::cout << "\nMacro Parameter Table (MPT):\n";
    std::cout << std::setw(15) << "Macro Name" << std::setw(20) << "Parameter Count" << std::setw(50) << "Parameters" << "\n";
    for (const auto& entry : mpt) {
        std::cout << std::setw(15) << entry.first 
                  << std::setw(20) << entry.second.paramCount
                  << std::setw(50);
        
        // Print the parameters in a single line
        for (size_t i = 0; i < entry.second.parameters.size(); ++i) {
            std::cout << entry.second.parameters[i];
            if (i != entry.second.parameters.size() - 1) {
                std::cout << ", ";  // Separate parameters with a comma
            }
        }
        std::cout << "\n";
    }
}

// Function to process the input file to extract macro parameters
void processInputFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    bool insideMacro = false;
    std::string currentMacro;
    std::vector<std::string> currentParams;
    
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
            currentParams.clear();  // Clear previous parameters if any
        } else if (word == "END") {
            insideMacro = false;
            // Add the macro and its parameters to the MPT
            addToMPT(currentMacro, currentParams);
        } else if (insideMacro) {
            // Collect macro parameters (everything after the macro name)
            std::string param;
            while (ss >> param) {
                currentParams.push_back(param);
            }
        }
    }

    inputFile.close();
}

int main() {
    std::string filename;
    
    // Ask for the input file name
    std::cout << "Enter the input file name: ";
    std::cin >> filename;

    // Process the input file and generate MPT
    processInputFile(filename);

    // Display MPT after processing the file
    printMPT();
    
    return 0;
}
