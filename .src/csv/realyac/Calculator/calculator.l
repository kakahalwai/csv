%{
    #include"calculator.tab.h"
    #include <stdio.h>
    extern FILE *yyin;
%}

%%
[0-9]+                     {yylval=atoi(yytext);return NUM;}
[\t\n]+                          ;
[()]                   return yytext[0];
.                           return yytext[0];
%%

int yywrap(){
    return 1;
}
