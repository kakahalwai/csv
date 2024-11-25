#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 256
#define MAX_PARAMS 10
#define MAX_MACRO_BODY 50

// Structure to store macro definition
typedef struct {
    char name[32];
    char params[MAX_PARAMS][32];
    char body[MAX_MACRO_BODY][MAX_LINE];
    int param_count;
    int body_lines;
} Macro;

// Function to trim whitespace
void trim(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;
    
    while(isspace(*start)) start++;
    while(end > start && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    memmove(str, start, strlen(start) + 1);
}

// Function to parse macro parameters
void parse_params(char *line, Macro *macro) {
    char *token = strtok(line, ",");
    macro->param_count = 0;
    
    while(token != NULL && macro->param_count < MAX_PARAMS) {
        trim(token);
        strcpy(macro->params[macro->param_count], token);
        macro->param_count++;
        token = strtok(NULL, ",");
    }
}

// Function to parse macro definition
void parse_macro(FILE *fp, Macro *macro) {
    char line[MAX_LINE];
    char *token;
    
    if(fgets(line, MAX_LINE, fp) != NULL) {
        line[strcspn(line, "\n")] = 0;
        token = strtok(line, " ");
        strcpy(macro->name, token);
        
        token = strtok(NULL, "\n");
        if(token) parse_params(token, macro);
    }
    
    macro->body_lines = 0;
    while(fgets(line, MAX_LINE, fp) != NULL && macro->body_lines < MAX_MACRO_BODY) {
        line[strcspn(line, "\n")] = 0;
        trim(line);
        
        if(strcmp(line, "MEND") == 0) break;
        
        strcpy(macro->body[macro->body_lines], line);
        macro->body_lines++;
    }
}

// Modified get_arg_value function to handle array of strings properly
char* get_arg_value(char *param, char *args[], int arg_count, char param_array[][32], int param_count) {
    for(int i = 0; i < param_count; i++) {
        if(strcmp(param, param_array[i]) == 0 && i < arg_count) {
            return args[i];
        }
    }
    return NULL;
}

// Function to expand macro call
void expand_macro(Macro *macro, char *args[], int arg_count, FILE *out_fp) {
    char expanded_line[MAX_LINE];
    char *param_value;
    
    for(int i = 0; i < macro->body_lines; i++) {
        strcpy(expanded_line, macro->body[i]);
        
        for(int j = 0; j < macro->param_count; j++) {
            param_value = get_arg_value(macro->params[j], args, arg_count,
                                      macro->params, macro->param_count);
            if(param_value) {
                char *pos = strstr(expanded_line, macro->params[j]);
                while(pos != NULL) {
                    int param_len = strlen(macro->params[j]);
                    int arg_len = strlen(param_value);
                    memmove(pos + arg_len, pos + param_len,
                           strlen(pos + param_len) + 1);
                    memcpy(pos, param_value, arg_len);
                    pos = strstr(pos + arg_len, macro->params[j]);
                }
            }
        }
        fprintf(out_fp, "\t%s\n", expanded_line);
    }
}

// Function to parse macro call
int parse_macro_call(char *line, char *args[]) {
    char *token;
    int arg_count = 0;
    
    token = strtok(line, " ");
    
    token = strtok(NULL, ",");
    while(token != NULL) {
        args[arg_count] = malloc(strlen(token) + 1);
        trim(token);
        strcpy(args[arg_count], token);
        arg_count++;
        token = strtok(NULL, ",");
    }
    
    return arg_count;
}

int main() {
    FILE *in_fp = fopen("input.asm", "r");
    FILE *out_fp = fopen("output.asm", "w");
    char line[MAX_LINE];
    Macro macro;
    int in_macro = 0;
    
    if(!in_fp || !out_fp) {
        printf("Error opening files\n");
        return 1;
    }
    
    // Write sample assembly code with macro usage
    const char *input_code = 
        "section .data\n"
        "    num1 dd 10\n"
        "    num2 dd 20\n"
        "    result dd 0\n"
        "\n"
        "MACRO\n"
        "ADD_VAL &X, &Y, &Z\n"
        "ADD &X, &Y\n"
        "MOV &Z, &X\n"
        "MEND\n"
        "\n"
        "section .text\n"
        "    global _start\n"
        "\n"
        "_start:\n"
        "    mov eax, [num1]    ; Load first number\n"
        "    mov ebx, [num2]    ; Load second number\n"
        "    ADD_VAL eax, ebx, [result]  ; Use macro to add and store\n"
        "    \n"
        "    ; Exit program\n"
        "    mov eax, 1      ; system call number (sys_exit)\n"
        "    mov ebx, 0      ; exit code 0 (success)\n"
        "    int 0x80        ; call kernel\n";

    // Write input code to file
    fprintf(in_fp, "%s", input_code);
    fseek(in_fp, 0, SEEK_SET);  // Reset file pointer to beginning
    
    // Process input file
    while(fgets(line, MAX_LINE, in_fp) != NULL) {  // Changed fp to in_fp
        line[strcspn(line, "\n")] = 0;
        trim(line);
        
        if(strlen(line) == 0) {
            fprintf(out_fp, "\n");
            continue;
        }
        
        if(strcmp(line, "MACRO") == 0) {
            in_macro = 1;
            parse_macro(in_fp, &macro);
            continue;
        }
        
        if(!in_macro) {
            char *temp_line = strdup(line);
            char *first_word = strtok(temp_line, " ");
            if(first_word && strcmp(first_word, macro.name) == 0) {
                char *args[MAX_PARAMS];
                int arg_count = parse_macro_call(line, args);
                expand_macro(&macro, args, arg_count, out_fp);
                
                for(int i = 0; i < arg_count; i++) {
                    free(args[i]);
                }
            } else {
                fprintf(out_fp, "%s\n", line);
            }
            free(temp_line);
        }
    }
    
    fclose(in_fp);
    fclose(out_fp);
    printf("Macro expansion completed. Check output.asm for results.\n");
    return 0;
}
