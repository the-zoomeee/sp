#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEYWORDS 44
#define MAX_OPERATORS 33
#define MAX_DELIMITERS 10
#define MAX_IDENTIFIER_LENGTH 100

// Define token types
typedef enum { KEYWORD, DELIMITER, OPERATOR, IDENTIFIER, ERROR } TokenType;

// List of all C language keywords
const char* keywords[MAX_KEYWORDS] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long",
    "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Alignas",
    "_Alignof", "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local"
};

// List of C operators
const char* operators[MAX_OPERATORS] = {
    "+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!",
    "&", "|", "^", "~", "<<", ">>", "++", "--", "%", "+=", "-=", "*=", "/=", "%=",
    "&=", "|=", "^=", "<<=", ">>="
};

// List of C delimiters
const char delimiters[MAX_DELIMITERS] = { '(', ')', '{', '}', '[', ']', ',', ';', ':', '.' };

// Symbol table for identifiers
char symbolTable[100][MAX_IDENTIFIER_LENGTH];
int symbolTableSize = 0;

// Function to check if a string is a keyword
int isKeyword(const char* str) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(str, keywords[i]) == 0) return i;
    }
    return -1;
}

// Function to check if a string is an operator
int isOperator(const char* str) {
    for (int i = 0; i < MAX_OPERATORS; i++) {
        if (strcmp(str, operators[i]) == 0) return i;
    }
    return -1;
}

// Function to check if a character is a delimiter
int isDelimiter(char ch) {
    for (int i = 0; i < MAX_DELIMITERS; i++) {
        if (ch == delimiters[i]) return i;
    }
    return -1;
}

// Function to display token information
void displayToken(int lineNo, const char* lexeme, TokenType tokenType, int tokenValue) {
    const char* tokenTypeStr;
    switch (tokenType) {
        case KEYWORD: tokenTypeStr = "KEYWORD"; break;
        case DELIMITER: tokenTypeStr = "DELIMITER"; break;
        case OPERATOR: tokenTypeStr = "OPERATOR"; break;
        case IDENTIFIER: tokenTypeStr = "IDENTIFIER"; break;
        case ERROR: tokenTypeStr = "ERROR"; break;
    }

    printf("%-10d%-20s%-15s%-10d\n", lineNo, lexeme, tokenTypeStr, tokenValue);
}

// Function to add an identifier to the symbol table
int addIdentifier(const char* identifier) {
    for (int i = 0; i < symbolTableSize; i++) {
        if (strcmp(symbolTable[i], identifier) == 0) {
            return i + 1;
        }
    }
    strcpy(symbolTable[symbolTableSize++], identifier);
    return symbolTableSize;
}

// Function to display the symbol table
void displaySymbolTable() {
    printf("\nSymbol Table for Identifiers:\n");
    printf("%-10s%s\n", "Index", "Identifier");
    printf("-----------------------------\n");
    for (int i = 0; i < symbolTableSize; i++) {
        printf("%-10d%s\n", i + 1, symbolTable[i]);
    }
}

// Lexical analyzer function
void lexicalAnalyzer(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int lineNo = 0;

    printf("%-10s%-20s%-15s%-10s\n", "Line No.", "Lexeme", "Token", "Token Value");
    printf("--------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        lineNo++;
        int i = 0;
        while (line[i] != '\0') {
            char lexeme[MAX_IDENTIFIER_LENGTH] = "";
            char ch = line[i];

            // Skip whitespace
            if (isspace(ch)) {
                i++;
                continue;
            }

            // Check if character is a delimiter
            int delimiterIndex = isDelimiter(ch);
            if (delimiterIndex != -1) {
                snprintf(lexeme, sizeof(lexeme), "%c", ch);
                displayToken(lineNo, lexeme, DELIMITER, delimiterIndex);
                i++;
                continue;
            }

            // Check if character is an operator
            lexeme[0] = ch;
            lexeme[1] = '\0';
            int operatorIndex = isOperator(lexeme);
            if (operatorIndex != -1) {
                displayToken(lineNo, lexeme, OPERATOR, operatorIndex);
                i++;
                continue;
            }

            // Check if it's an identifier or keyword
            if (isalpha(ch) || ch == '_') {
                int j = 0;
                while (isalnum(ch) || ch == '_') {
                    lexeme[j++] = ch;
                    ch = line[++i];
                }
                lexeme[j] = '\0';

                int keywordIndex = isKeyword(lexeme);
                if (keywordIndex != -1) {
                    displayToken(lineNo, lexeme, KEYWORD, keywordIndex);
                } else {
                    int idIndex = addIdentifier(lexeme);
                    displayToken(lineNo, lexeme, IDENTIFIER, idIndex);
                }
                continue;
            }

            // If none of the above, it's an error
            snprintf(lexeme, sizeof(lexeme), "%c", ch);
            displayToken(lineNo, lexeme, ERROR, -1);
            i++;
        }
    }

    fclose(file);

    // Display the symbol table after lexical analysis
    displaySymbolTable();
}

int main() {
    char filename[100];
    printf("Enter the filename: ");
    scanf("%s", filename);
    lexicalAnalyzer(filename);

    return 0;
}
