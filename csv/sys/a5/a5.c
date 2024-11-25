#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_PARAMS 3
#define MAX_PARAM_LEN 32

// Function to trim whitespace from a string
void trim(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;
    
    while(isspace(*start)) start++;
    while(end > start && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    memmove(str, start, strlen(start) + 1);
}

// Function to parse macro parameters
int parse_params(char *line, char params[MAX_PARAMS][MAX_PARAM_LEN]) {
    char *token;
    int count = 0;
    
    // Skip macro name
    token = strtok(line, ",");
    if (token) {
        char *comma = strchr(token, ' ');
        if (comma) token = comma + 1;
        trim(token);
        if (token[0] == '&') {
            strncpy(params[count], token + 1, MAX_PARAM_LEN - 1);
            params[count][MAX_PARAM_LEN - 1] = '\0';
            count++;
        }
    }
    
    // Get remaining parameters
    while ((token = strtok(NULL, ",")) && count < MAX_PARAMS) {
        trim(token);
        if (token[0] == '&') {
            strncpy(params[count], token + 1, MAX_PARAM_LEN - 1);
            params[count][MAX_PARAM_LEN - 1] = '\0';
            count++;
        }
    }
    
    return count;
}

// Function to replace parameters in a line
void replace_params(char *line, char params[MAX_PARAMS][MAX_PARAM_LEN], 
                   char values[MAX_PARAMS][MAX_PARAM_LEN]) {
    for (int i = 0; i < MAX_PARAMS; i++) {
        if (strlen(params[i]) > 0) {
            char param[MAX_PARAM_LEN + 2];
            snprintf(param, sizeof(param), "&%s", params[i]);
            
            char *pos = strstr(line, param);
            if (pos) {
                int param_len = strlen(param);
                int value_len = strlen(values[i]);
                memmove(pos + value_len, pos + param_len, 
                        strlen(pos + param_len) + 1);
                memcpy(pos, values[i], value_len);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input.asm>\n", argv[0]);
        return 1;
    }
    
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Error opening input file\n");
        return 1;
    }
    
    FILE *output = fopen("output.asm", "w");
    if (!output) {
        fclose(input);
        printf("Error creating output file\n");
        return 1;
    }
    
    char line[MAX_LINE];
    int in_macro = 0;
    char macro_params[MAX_PARAMS][MAX_PARAM_LEN] = {0};
    char current_values[MAX_PARAMS][MAX_PARAM_LEN] = {0};
    
    while (fgets(line, sizeof(line), input)) {
        char temp_line[MAX_LINE];
        strncpy(temp_line, line, MAX_LINE);
        trim(temp_line);
        
        if (strncmp(temp_line, "ADD_VAL", 7) == 0) {
            // Parse macro parameters
            parse_params(temp_line, current_values);
            
            // Output expanded macro
            fprintf(output, "ADD %s, %s\n", current_values[0], current_values[1]);
            fprintf(output, "MOV %s, %s\n", current_values[2], current_values[0]);
        } else if (strncmp(temp_line, "MACRO", 5) == 0) {
            in_macro = 1;
            // Skip macro definition lines
        } else if (strncmp(temp_line, "MEND", 4) == 0) {
            in_macro = 0;
            memset(macro_params, 0, sizeof(macro_params));
        } else if (!in_macro) {
            fputs(line, output);
        }
    }
    
    fclose(input);
    fclose(output);
    printf("Macro expansion completed. Output written to output.asm\n");
    
    return 0;
}
