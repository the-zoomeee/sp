#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures for Literal and Pool Tables
typedef struct {
    int index;
    char literal[10];
    int address;
} Literal;

typedef struct {
    int poolIndex;
    int literalIndex; // Starting index of literals in this pool
} Pool;

// Global variables for Literal Table and Pool Table
Literal litTable[100];
Pool poolTable[100];
int litCount = 0, poolCount = 0;
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

// Function to add a new pool to the Pool Table
void addPool() {
    poolTable[poolCount].poolIndex = poolCount + 1;
    poolTable[poolCount].literalIndex = litCount + 1; // Points to the next literal
    poolCount++;
}

// Function to process a single line of assembly code
void processLine(char *line) {
    char label[10], opcode[10], operand[10];
    int tokens = sscanf(line, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        locationCounter = atoi(operand);
    } else if (strcmp(opcode, "LTORG") == 0 || strcmp(opcode, "END") == 0) {
        // Assign addresses to literals in the current pool
        for (int i = poolTable[poolCount - 1].literalIndex - 1; i < litCount; i++) {
            if (litTable[i].address == -1) {
                litTable[i].address = locationCounter++;
            }
        }
        addPool(); // Create a new pool for subsequent literals
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

    // Initialize Pool Table with the first pool
    addPool();

    while (fgets(line, sizeof(line), inputFile)) {
        processLine(line);
    }

    fclose(inputFile);

    // Display the Pool Table
    printf("\nPool Table:\nIndex\tLiteral Index\n");
    for (int i = 0; i < poolCount; i++) {
        printf("%d\t%d\n", poolTable[i].poolIndex, poolTable[i].literalIndex);
    }

    return 0;
}
