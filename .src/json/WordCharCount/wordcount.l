%{
#include <stdio.h>

int word_count = 0;
int char_count = 0;
%}
%option noyywrap
%%

[ \t\n]+         { /* Whitespace: skip but separates words */ }
[a-zA-Z0-9_]+    { word_count++; char_count += yyleng; }  // yyleng = length of current match
.                { char_count++; } // Count any single punctuation/symbol as a character

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("File open failed");
            return 1;
        }
        yyin = file;
    }

    yylex();

    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", char_count);
    return 0;
}
