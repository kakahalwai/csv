%{
#include<stdio.h>
#include<stdlib.h>
int lineno = 1;
int yylex();
void yyerror(const char *s);
extern FILE *yyin;
%}

%token ID NUM CONDI_OP OPERATOR WHILE
//Note to use single quotes only

%%
start : while_smt ;

while_smt : WHILE '(' condition ')' '{' statement_list '}' 
{
    printf("While Statment Syntax Valid")
}

condition : ID CONDI_OP ID | ID CONDI_OP NUM | NUM CONDI_OP ID 

statement_list : | statement_list statement

statement : ID OPERATOR expression ';' | NUM OPERATOR expression ';' | expression ';' 

expression : ID | NUM | expression OPERATOR expression ;
%%


void yyerror(const char *s){
    fprintf(stderr, "Syntax error at line %d: %s\n", lineno, s);
    exit(1);}

int main (int argc, char **argv){
    if (argc >1 ){
        yyin = fopen(argv[1],"r");
    }
    return yyparse();

}