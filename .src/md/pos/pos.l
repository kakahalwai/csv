%{

#include<stdio.h>

%}

%option noyywrap

%%
[aA]|[aA]n|[tT]he                        { printf("ARTICLE: %s\n", yytext); }
[inIN]|[onON]|[atAT]|[byBY]|[toTO]      { printf("PREPOSITION: %s\n", yytext); }
[Hh]e|[Ss]he|[Ii]t|[Tt]hey|[Ww]e|[Yy]ou { printf("PRONOUN: %s\n", yytext); }
[Ww]as|[Ii]s|[Aa]re|[Rr]un|[Gg]o|[Ss]ee { printf("VERB: %s\n", yytext); }

[a-zA-Z]+                                { printf("NOUN: %s\n", yytext); }
[.,?!]                                   { printf("PUNCTUATION: %s\n", yytext); }
[ \t\n]+                                 ; // ignore whitespace
.                                        { printf("UNKNOWN: %s\n", yytext); }
%%

int main() {

    yylex();
    return 0;

}
