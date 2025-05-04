
%{
#include "y.tab.h"
extern FILE *yyin;
%}

%%
[ \t]+                ;    // Ignore spaces and tabs
[a-z]                 { return ID; } // Single lowercase character
[\+\-\*\/]            { return yytext[0]; } // + - * /
\(                    { return '('; }
\)                    { return ')'; }
\n                    ;
.                     { return yytext[0]; }
%%
