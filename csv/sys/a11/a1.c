#include <stdio.h>
#include <string.h>
#include <ctype.h> // For isspace() function

#define MAX_ENTRIES 7
#define MAX_LINES 100

struct MOT_Entry
{
    char operation[20];
    char type[3];
    int opcode;
};

struct Symbol_Entry
{
    char label[20];
    int location;
};

struct LC_Entry
{
    int line;
    int LC_value;
};

int checkCommandValidity(char *mnemonic, struct MOT_Entry motTable[], int tableSize)
{
    for (int i = 0; i < tableSize; i++)
    {
        if (strcmp(mnemonic, motTable[i].operation) == 0)
        {
            return 1; // Command is valid
        }
    }
    return 0; // Command is invalid
}

int hasLabel(char *word)
{
    size_t length = strlen(word);
    return (length > 0 && word[length - 1] == ':');
}

void splitLine(char *line, char *label, char *mnemonic, char *operand1, char *operand2)
{
    // Reset the values
    label[0] = '\0';
    mnemonic[0] = '\0';
    operand1[0] = '\0';
    operand2[0] = '\0';

    // Get the first word (token) from the line
    char *token = strtok(line, " \t\n");

    // Check if the first word is a label
    if (hasLabel(token))
    {
        strcpy(label, token);
        token = strtok(NULL, " \t\n,"); // Move to the next word (mnemonic)
    }

    // Get mnemonic and operands (if present)
    if (token != NULL)
    {
        strcpy(mnemonic, token);
        token = strtok(NULL, " \t\n,"); // Move to the next word (operand1)
    }
    if (token != NULL)
    {
        strcpy(operand1, token);
        token = strtok(NULL, " \t\n,"); // Move to the next word (operand2)
    }
    if (token != NULL)
    {
        strcpy(operand2, token);
    }
}

int getInstructionSize(char *mnemonic, struct MOT_Entry motTable[], int tableSize)
{
    for (int i = 0; i < tableSize; i++)
    {
        if (strcmp(mnemonic, motTable[i].operation) == 0)
        {
            return 1; // Assuming each instruction occupies one memory unit
        }
    }
    return 0; // Invalid instruction, assuming size 0
}

int main()
{
    // MOT array with 7 elements
    struct MOT_Entry motTable[MAX_ENTRIES] = {
        {"START", "AD", 0},
        {"MOV", "IS", 2},
        {"ADD", "IS", 2},
        {"SUM", "IS", 2},
        {"MUL", "IS", 2},
        {"END", "AD", 2},
        {"EQU", "AD", 2}};

    // Displaying MOT table in tabular format
    printf("MOT Table:\n");
    printf("-----------------------------------------\n");
    printf("%-10s %-5s %s\n", "Operation", "Type", "Opcode");
    printf("-----------------------------\n");
    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        printf("%-10s %-5s %d\n", motTable[i].operation, motTable[i].type, motTable[i].opcode);
    }
    printf("-----------------------------------------");
    printf("\nCode from Text file.\n");
    printf("-----------------------------------------\n");

    FILE *file;
    char line[256]; // Assuming each line in the file is not longer than 255 characters
    char lines[MAX_LINES][256];
    int lineCount = 0;

    // Open the file in read mode
    file = fopen("A1(asm code).txt", "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    // Read the file and store each line in the array
    while (lineCount < MAX_LINES && fgets(line, sizeof(line), file) != NULL)
    {
        strcpy(lines[lineCount], line);
        lineCount++;
    }

    // Close the file
    fclose(file);

    // Display the content of the text file
    for (int i = 0; i < lineCount; i++)
    {
        printf("%s", lines[i]);
    }
    printf("-----------------------------------------\n");

    // Parse and display each line in the file
    struct Symbol_Entry symbolTable[MAX_LINES];
    struct LC_Entry lcTable[MAX_LINES];
    int locationCounter = 0; // Location counter to keep track of memory locations
    int symbolCount = 0;     // Counter for the symbol table
    int lcCount = 0;         // Counter for the LC table

    for (int i = 0; i < lineCount; i++)
    {
        char label[20] = "", mnemonic[20], operand1[20], operand2[20];
        splitLine(lines[i], label, mnemonic, operand1, operand2);

        // Display parsed information
        printf("Line %d:\n Label: %s, Mnemonic: %s, Operand1: %s, Operand2: %s\n", i + 1, label, mnemonic, operand1, operand2);

        // Check if the mnemonic is valid and display validity message
        if (mnemonic[0] != '\0')
        {
            int isValidMnemonic = checkCommandValidity(mnemonic, motTable, MAX_ENTRIES);
            if (isValidMnemonic)
            {
                printf("Valid Mnemonic: %s, Type: %s, Opcode: %d\n\n", mnemonic, motTable[i].type, motTable[i].opcode);
            }
            else
            {
                printf("Error: Invalid mnemonic - %s\n\n", mnemonic);
            }
        }

        // Update the symbol table if a label is present
        if (label[0] != '\0')
        {
            // Check if the label is not already present in the symbol table
            int labelExists = 0;
            for (int j = 0; j < symbolCount; j++)
            {
                if (strcmp(label, symbolTable[j].label) == 0)
                {
                    labelExists = 1;
                    break;
                }
            }

            // If the label is not present, add it to the symbol table
            if (!labelExists)
            {
                strcpy(symbolTable[symbolCount].label, label);
                symbolTable[symbolCount].location = locationCounter;
                symbolCount++;
            }
        }

        // Add the valid mnemonic and its corresponding LC to the LC table
        if (mnemonic[0] != '\0')
        {
            lcTable[lcCount].line = i + 1;
            lcTable[lcCount].LC_value = locationCounter;
            lcCount++;
        }

        // Increment the location counter based on the mnemonic
        int instructionSize = getInstructionSize(mnemonic, motTable, MAX_ENTRIES);
        locationCounter += instructionSize;
    }

    // Display the symbol table
    printf("\nSymbol Table:\n");
    printf("-----------------------------------------\n");
    printf("%-10s %s\n", "Label", "Location");
    printf("-----------------------------\n");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("%-10s %d\n", symbolTable[i].label, symbolTable[i].location);
    }
    printf("-----------------------------------------\n");

    // Display the LC table
    printf("\nLC Table:\n");
    printf("-----------------------------------------\n");
    printf("%-5s %-5s\n", "Line", "LC Value");
    printf("-----------------\n");
    for (int i = 0; i < lcCount; i++)
    {
        printf("%-5d %-5d\n", lcTable[i].line, lcTable[i].LC_value);
    }
    printf("-----------------------------------------\n");

    return 0;
}
