%{
#include "dowhile.tab.h"
extern int line;
%}

%option noyywrap

%%

[\t ]+              ;
\n                  { line++; }

"do"                { return DO; }
"while"             { return WHILE; }

"=="|"<="|">="|"<"|">" { return RELOP; }

[a-zA-Z_][a-zA-Z0-9_]*   { return ID; }
[0-9]+                  { return NUM; }

"="             { return '='; }
";"             { return ';'; }
"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }

.               { return yytext[0]; }

%%
