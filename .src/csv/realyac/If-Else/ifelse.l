%{
#include "ifelse.tab.h"
#include <stdio.h>
extern int line;
%}

%option noyywrap

%%

[\t ]+              ;     // Skip whitespace
\n                  { line++; }

"if"                { return IF; }
"else"              { return ELSE; }

"=="|"<="|">="|"<"|">" { return RELOP; }

[a-zA-Z_][a-zA-Z0-9_]*   { return ID; }
[0-9]+              { return NUM; }

"="                 { return '='; }
";"                 { return ';'; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }

.                   { return yytext[0]; }

%%
