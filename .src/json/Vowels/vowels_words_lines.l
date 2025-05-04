%{
#include <stdio.h>
int line = 1;
extern FILE *yyin;
%}

%%
[aAeEiIoOuU]    { printf("Vowel '%s' found at line %d\n", yytext, line); }
\n              { line++; }          // Increment line on newline
.               ;                   // Ignore other characters
%%

int yywrap() {
    return 1;
}

int main() {
    yyin = fopen("inputv.txt", "r");
    if (!yyin) {
        perror("Cannot open file");
        return 1;
    }
    yylex();
    fclose(yyin);
    return 0;
}
