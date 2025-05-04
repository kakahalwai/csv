%{
#include <stdio.h>

int count = 0;
%}

%%

Vishwakarma     { count++; }

.|\n            { /* Ignore other characters */ }

%%

int main(int argc, char **argv) {
    // Read from file if provided
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("File open failed");
            return 1;
        }
        yyin = file;
    }

    yylex();  // Run lexer
   
    printf("The word 'Vishwakarma' occurred %d times.\n", count);
    return 0;
}

int yywrap()

{
return 1;

}
