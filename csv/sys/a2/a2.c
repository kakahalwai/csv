#include <stdio.h>
#include <string.h>

struct MOTEntry {
    char mnemonic[10];
    char opcode[4]; // Increased size to accommodate opcode
    int size;
    char type[15];
};

struct LCEntry {
    int address;
    int value;
};

struct LiteralEntry {
    char value[10];
    int index; // Index of the literal in the pool table, -1 if not in the pool
};

struct ICEntry {
    char address[10]; // Increased size for better formatting
    char code[50];    // Increased size to accommodate register values
};

int getRegisterValue(char reg) {
    if (reg == 'A') {
        return 1;
    } else if (reg == 'B') {
        return 2;
    } else if (reg == 'C') {
        return 3;
    }
    return 0; // Return 0 if not a recognized register
}

int isLiteral(const char *operand) {
    return (operand[0] == '=');
}

int main() {
    FILE *file;
    char line[100]; // Assuming a maximum line length of 100 characters
    file = fopen("A2.txt", "r");
    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1; // Exit with an error code
    }
    printf("Content of the 'ASM txt' file:\n");
    printf("------------------------------------\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    printf("\n");
    printf("------------------------------------\n");
    fclose(file);

    struct MOTEntry motTable[] = {
        {"START", "001", 1, "AD"},
        {"MOV", "01", 2, "IS"},
        {"ADD", "02", 2, "IS"},
        {"LTORG", "002", -1, "AD"},
        {"MUL", "03", 2, "IS"},
        {"ADD", "02", 2, "IS"},
        {"LTORG", "002", -1, "AD"},
        {"MOV", "01", 2, "IS"},
        {"ADD", "02", 2, "AD"},
        {"END", "003", 2, "AD"}
    };

    printf("\nMOT Table:\n");
    printf("-------------------------------------------\n");
    printf("| %-8s | %-6s | %-4s | %-12s |\n", "Mnemonic", "Opcode", "Size", "Type");
    printf("|----------|--------|------|--------------|\n");
    for (int i = 0; i < sizeof(motTable) / sizeof(motTable[0]); i++) {
        printf("| %-8s | %-6s | %-4d | %-12s |\n",
               motTable[i].mnemonic, motTable[i].opcode, motTable[i].size, motTable[i].type);
    }

    struct ICEntry icTable[100]; // Assuming a maximum of 100 instructions
    int icTableIndex = 0;
    int addressCounter = 1000; // Starting address

    struct LiteralEntry literalTable[100]; // Assuming a maximum of 100 literals
    int literalTableIndex = 0;

    int poolTable[100]; // Assuming a maximum of 100 entries in the pool table
    int poolTableIndex = 0;

    struct LCEntry lcTable[100]; // Assuming a maximum of 100 instructions
    int lcTableIndex = 0;
    int lcValue = 200; // Starting LC value

    file = fopen("A2.txt", "r");
    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1; // Exit with an error code
    }

    int literalCounter = 0; // Counter to keep track of the literals in the assembly code

    while (fgets(line, sizeof(line), file) != NULL) {
        icTable[icTableIndex].address[0] = '\0'; // Address initially empty

        char *operand = strchr(line, ',');
        if (operand) {
            *operand = '\0'; // Separate opcode/mnemonic from operand
            operand++;       // Move operand pointer to the next character
        }

        for (int i = 0; i < sizeof(motTable) / sizeof(motTable[0]); i++) {
            if (strstr(line, motTable[i].mnemonic)) {
                strcpy(icTable[icTableIndex].code, "");
                strcat(icTable[icTableIndex].code, "(");
                strcat(icTable[icTableIndex].code, motTable[i].type);
                strcat(icTable[icTableIndex].code, ",");
                strcat(icTable[icTableIndex].code, motTable[i].opcode);
                strcat(icTable[icTableIndex].code, ")");

                if (operand) {
                    int regValue = getRegisterValue(*operand);
                    if (regValue != 0) {
                        sprintf(icTable[icTableIndex].code + strlen(icTable[icTableIndex].code), " (R,%d)", regValue);
                    }
                }

                if (strcmp(motTable[i].type, "AD") == 0) {
                    sprintf(icTable[icTableIndex].code + strlen(icTable[icTableIndex].code), ",(%s)", motTable[i].opcode);
                    if (strcmp(motTable[i].mnemonic, "LTORG") != 0) {
                        sprintf(icTable[icTableIndex].address, "(C,%d)", addressCounter);
                        poolTable[poolTableIndex++] = literalCounter; // Store literal counter in pool table
                    }
                }

                if (operand && isLiteral(operand)) {
                    strcpy(literalTable[literalTableIndex].value, operand + 1); // Skip the '=' sign
                    literalTable[literalTableIndex].index = literalCounter; // Set the index as the current literalCounter
                    literalTableIndex++;
                    literalCounter++; // Increment the literal counter
                }

                if (strcmp(motTable[i].mnemonic, "LTORG") == 0) {
                    while (poolTableIndex > 0) {
                        literalTable[poolTable[--poolTableIndex]].index = literalCounter;
                    }
                }

                addressCounter += motTable[i].size;
                break;
            }
        }

        lcTable[lcTableIndex].address = lcValue;
        lcTable[lcTableIndex].value = lcValue;

        for (int i = 0; i < sizeof(motTable) / sizeof(motTable[0]); i++) {
            if (strstr(line, motTable[i].mnemonic)) {
                lcValue += motTable[i].size;
                break;
            }
        }

        icTableIndex++;
        lcTableIndex++;
    }

    fclose(file);

    printf("\nIntermediate Code (IC) Table:\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < icTableIndex; i++) {
        printf("| %-25s | %-25s |\n", icTable[i].address, icTable[i].code);
    }

    printf("\nLC Table:\n");
    printf("-----------------\n");
    for (int i = 0; i < lcTableIndex; i++) {
        printf("| Address: %4d | Value: %4d |\n", lcTable[i].address, lcTable[i].value);
    }

    printf("\nLiteral Table:\n");
    printf("-----------------\n");
    for (int i = 0; i < literalTableIndex; i++) {
        printf("| Index: %4d | Value: %-5s |\n", literalTable[i].index, literalTable[i].value);
    }

    printf("\nPool Table:\n");
    printf("-----------------\n");
    for (int i = 0; i < poolTableIndex; i++) {
        printf("| Index: %4d |\n", poolTable[i]);
    }

    return 0;
}
