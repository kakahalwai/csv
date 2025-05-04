%{
#include <stdio.h>
#include <stdlib.h>
int line = 1;
void yyerror(const char *s);
int yylex(void);
%}

%token DO WHILE ID NUM RELOP

%%

stmt : DO '{' stmt_list '}' WHILE '(' cond ')' ';'   { printf("✅ Valid do-while statement\n"); }
     ;

cond : ID RELOP NUM
     | NUM RELOP ID
     | ID RELOP ID
     ;

stmt_list : /* empty */
          | stmt_list simple_stmt
          ;

simple_stmt : ID '=' NUM ';'
            ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "❌ Syntax error at line %d: %s\n", line, s);
}

int main(int argc, char **argv) {
    extern FILE *yyin;
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Cannot open file");
            return 1;
        }
    } else {
        printf("Enter a do-while loop:\n");
    }
    return yyparse();
}
