%{
#include "while.tab.h"
extern FILE *yyin;
extern int line;
%}

%%

[\t\r]+                ;             // Ignore whitespace
\n                     { line++; }

"while"                { return WHILE; }
"&&"                   { return AND; }
"||"                   { return OR; }
"!"                    { return NOT; }

"==" | "!=" | "<=" | ">=" | "<" | ">"   { return RELOP; }

[{}();]                { return yytext[0]; }
[a-z]+                 { return ID; }
[0-9]+                 { return NUM; }
.                      { return yytext[0]; }

%%

int yywrap() { return 1; }
