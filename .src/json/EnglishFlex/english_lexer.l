%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int line = 1;
extern FILE *yyin;

void print_token(const char* lexeme, const char* token) {
    printf("%-10d %-15s %s\n", line, lexeme, token);
}
%}

%%

[aAeEiIoOuU]              { print_token(yytext, "VOWEL"); }
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z]  { print_token(yytext, "CONSONANT"); }
[0-9]+                    { print_token(yytext, "NUMBER"); }
[A-Za-z]+                 { print_token(yytext, "WORD"); }
[ \t]+                    ; // Ignore spaces/tabs
[\n]                      { line++; }
[.,!?;:'"()]              { print_token(yytext, "PUNCTUATION"); }
.                         { print_token(yytext, "UNKNOWN"); }

%%

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Cannot open file");
        return 1;
    }

    printf("\nLine No   Lexeme           Token\n");
    printf("----------------------------------------\n");

    yylex();
    fclose(yyin);
    return 0;
}
