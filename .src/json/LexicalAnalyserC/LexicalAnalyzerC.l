%{
#include<stdio.h>
extern FILE *yyin;
int lineno = 1;
%}
%option noyywrap
%%
"int"|"float"|"return"|"if"|"else"|"for"        { printf("Line %d: Keyword: %s\n", lineno, yytext); }
[0-9]+\\.[0-9]+                                 { printf("Line %d: Floating Number: %s\n", lineno, yytext); }
[0-9]+                                          { printf("Line %d: Number: %s\n", lineno, yytext); }
[a-zA-Z_][a-zA-Z0-9_]*                          { printf("Line %d: Identifier: %s\n", lineno, yytext); }
"+"|"-"|"*"|"/"|"="|">"                         { printf("Line %d: Operator: %s\n", lineno, yytext); }
[\(\)\{\}\[\],;]                                { printf("Line %d: Delimiter: %s\n", lineno, yytext); }
[ \t]+                                          { /* Ignore whitespace */ }
\n                                              { lineno++; }
.                                               { printf("Line %d: Unknown: %s\n", lineno, yytext); }
%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            printf("Cannot open file %s\n", argv[1]);
            return -1;
        }
        yyin = fp;
    }
    yylex();
    return 0;
}