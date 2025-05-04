%{
#include<stdio.h>
#include<stdlib.h>
extern FILE *yyin;
int valid = 1;
%}

%token ID

%%

E : E '+' T
  | E '-' T
  | T
  ;

T : T '*' F
  | T '/' F
  | F
  ;

F : '(' E ')'
  | ID
  ;

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            printf("Cannot open file: %s\n", argv[1]);
            return 1;
        }
        yyin = fp;
    }

    if (yyparse() == 0 && valid) {
        printf("Valid Expression!\n");
    } else {
        printf("Syntax Error!\n");
    }
    return 0;
}

void yyerror(const char *s) {
    valid = 0;
}

int yywrap() {
    return 1;
}
