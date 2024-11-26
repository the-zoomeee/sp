#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Literal Table structure
typedef struct {
    int index;
    char literal[10];
    int address;
} Literal;

// Global variables for the Literal Table
Literal litTable[100];
int litCount = 0;
int locationCounter = 0;

// Function to add literals to the Literal Table
void addLiteral(char *literal) {
    for (int i = 0; i < litCount; i++) {
        if (strcmp(litTable[i].literal, literal) == 0) {
            return; // Literal already exists
        }
    }
    strcpy(litTable[litCount].literal, literal);
    litTable[litCount].address = -1; // Address resolved later
    litTable[litCount].index = litCount + 1;
    litCount++;
}

// Function to process a single line of assembly code
void processLine(char *line) {
    char label[10], opcode[10], operand[10];
    int tokens = sscanf(line, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        locationCounter = atoi(operand);
    } else if (strcmp(opcode, "LTORG") == 0 || strcmp(opcode, "END") == 0) {
        // Assign addresses to literals in the current pool
        for (int i = 0; i < litCount; i++) {
            if (litTable[i].address == -1) { // Only resolve unresolved literals
                litTable[i].address = locationCounter++;
            }
        }
    } else if (operand[0] == '=') {
        addLiteral(operand); // Add operand to Literal Table if it's a literal
    } else {
        locationCounter++;
    }
}

int main() {
    char line[100];
    FILE *inputFile = fopen("source.asm", "r");

    if (!inputFile) {
        printf("Error: Could not open the source file.\n");
        return 1;
    }

    printf("Processing assembly code...\n");

    while (fgets(line, sizeof(line), inputFile)) {
        processLine(line);
    }

    fclose(inputFile);

    // Display the Literal Table
    printf("\nLiteral Table:\nIndex\tLiteral\tAddress\n");
    for (int i = 0; i < litCount; i++) {
        printf("%d\t%s\t%d\n", litTable[i].index, litTable[i].literal, litTable[i].address);
    }

    return 0;
}
