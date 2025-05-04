%{
#include <stdio.h>
#include <stdlib.h>

FILE *output;
%}


%option noyywrap

%%
"printf"              { fprintf(output, "cout <<"); }
"scanf"		      { fprintf(output, "cin >>"); }
[()]		      ;
.|\n                  { fprintf(output, "%s", yytext); }

%%

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input.c> <output.c>\n", argv[0]);
        exit(1);
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Input file error");
        exit(1);
    }

    output = fopen(argv[2], "w");
    if (!output) {
        perror("Output file error");
        exit(1);
    }

    yyin = input;
    yylex();

    fclose(input);
    fclose(output);
    return 0;
}

