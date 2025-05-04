%{
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
int lineno = 1;
int yylex();
void yyerror(const char *s);
extern FILE *yyin;
%}


%token  NUM
%left '+' '-' 
%left '*' '/'
%right '^'
%%

S : E        { printf("= %d\n", $1); }
  ;

E : E '+' E      { $$ = $1 + $3; }
  | E '-' E      { $$ = $1 - $3; }
  | E '*' E      { $$ = $1 * $3; }
  | E '/' E      { $$ = $1 / $3; }
  | '(' E ')'    { $$ = $2; }
  | E '^' E      { $$ = pow($1,$2); }
  | NUM          { $$ = $1; }
  ;


%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", lineno, s);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    }
    return yyparse();
}
