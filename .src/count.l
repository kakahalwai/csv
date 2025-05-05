%{
#include <stdio.h>

int vowels = 0, consonants = 0;
int uppercase = 0, lowercase = 0;
%}

%%
[aAeEiIoOuU]     { vowels++; 
                   if(yytext[0] >= 'A' && yytext[0] <= 'Z') uppercase++;
                   else lowercase++; 
                 }

[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z]  {
                   consonants++;
                   if(yytext[0] >= 'A' && yytext[0] <= 'Z') uppercase++;
                   else lowercase++;
                 }

.|\n             ; // Ignore everything else
%%

int main() {
    printf("Enter a sentence: ");
    yylex();
    printf("\nVowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    printf("Uppercase letters: %d\n", uppercase);
    printf("Lowercase letters: %d\n", lowercase);
    return 0;
}

int yywrap() {
    return 1;
}

